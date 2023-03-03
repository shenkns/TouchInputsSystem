// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Components/TouchInputsComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Inputs/TouchInputComponent.h"
#include "Data/TouchInputSlotData.h"
#include "TouchInputsConfigurationObjects/TouchInputObject.h"
#include "Module/TouchInputsSystemModule.h"
#include "LogSystem.h"

UTouchInputsComponent::UTouchInputsComponent()
{
	SetIsReplicatedByDefault(true);
}

TArray<UTouchInputComponent*> UTouchInputsComponent::FilterByActive(TArray<UTouchInputComponent*> Components)
{
	return Components.FilterByPredicate([&](const UTouchInputComponent* Component)
	{
		return Component && Component->IsActive();
	});
}

TArray<UTouchInputComponent*> UTouchInputsComponent::FilterByBounds(TArray<UTouchInputComponent*> Components, FVector Location)
{
	return Components.FilterByPredicate([&](const UTouchInputComponent* Component)
	{
		return Component->CheckBounds(Location);
	});
}

TArray<UTouchInputComponent*> UTouchInputsComponent::SortByPriority(TArray<UTouchInputComponent*> Components)
{
	Components.Sort([&](const UTouchInputComponent& ComponentA, const UTouchInputComponent& ComponentB)
	{
		return ComponentA.Priority > ComponentB.Priority;
	});

	return Components;
}

APlayerController* UTouchInputsComponent::GetOwningPlayerController() const
{
	if(const APawn* OwningPawn = GetOwner<APawn>())
	{
		if(APlayerController* OwningController = OwningPawn->GetController<APlayerController>())
		{
			return OwningController;
		}
		else return nullptr;
	}
	else if(APlayerController* OwningController = GetOwner<APlayerController>())
	{
		return OwningController;
	}
	else return nullptr;
}

void UTouchInputsComponent::BeginPlay()
{
	Super::BeginPlay();

	/*if(GetOwner()->HasAuthority())
	{
		ClientBeginPlay();

		return;
	}

	if(GetOwner()->GetInstigator()->IsLocallyControlled() && !bPossessed)
	{
		PossessionUpdated();

		return;
	}*/
}

void UTouchInputsComponent::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if(NewPawn == GetOwner()) return;

	bPossessed = false;

	for(UTouchInputComponent* TouchInput : TouchInputComponents)
	{
		TouchInput->K2_DestroyComponent(TouchInput);
	}

	if(!NewPawn) return;
	
	if(APlayerController* Controller = NewPawn->GetController<APlayerController>())
	{
		Controller->OnPossessedPawnChanged.RemoveDynamic(this, &UTouchInputsComponent::OnPawnChanged);
	}
}

void UTouchInputsComponent::ClientBeginPlay_Implementation()
{
	if(GetOwner()->GetInstigator()->IsLocallyControlled() && !bPossessed)
	{
		PossessionUpdated();
	}
}

void UTouchInputsComponent::PossessionUpdated()
{
	bPossessed = true;
		
	Init();
		
	GetOwningPlayerController()->OnPossessedPawnChanged.AddUniqueDynamic(this, &UTouchInputsComponent::OnPawnChanged);

	DEBUG_MESSAGE(true, LogTouchInputsSystem, "%s", *GetOwningPlayerController()->GetName())
}

void UTouchInputsComponent::Init()
{
	for(const TTuple<UTouchInputSlotData*, UTouchInputObject*>& TouchInput : TouchInputsConfiguration)
	{
		TouchInputComponents.Add(TouchInput.Value->AddTouchInputComponent(GetOwner(), TouchInput.Key));

		LOG(LogTouchInputsSystem, "For %s Input Object In %s Slot Created %s Input Component", *TouchInput.Value->GetName(), *TouchInput.Key->Name.ToString(), *TouchInputComponents.Last(0)->GetName())
	}

	BindTouchEvents();

	LOG(LogTouchInputsSystem, "%s Touch Events Binded", *GetName())
}

void UTouchInputsComponent::BindTouchEvents()
{
	const APlayerController* OwningPlayerController = GetOwningPlayerController();

	if(!OwningPlayerController) return;
	
	OwningPlayerController->InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &UTouchInputsComponent::OnTouchPressed);
	LOG(LogTouchInputsSystem, "Touch Press Binded")
	
	OwningPlayerController->InputComponent->BindTouch(EInputEvent::IE_Released, this, &UTouchInputsComponent::OnTouchReleased);
	LOG(LogTouchInputsSystem, "Touch Release Binded")
	
	OwningPlayerController->InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &UTouchInputsComponent::OnTouchMoved);
	LOG(LogTouchInputsSystem, "Touch Move Binded")
}

void UTouchInputsComponent::BindUnPossess()
{
	if(APlayerController* OwningController = GetOwningPlayerController())
	{
		OwningController->OnPossessedPawnChanged.AddUniqueDynamic(this, &UTouchInputsComponent::OnPawnChanged);
	}
}

void UTouchInputsComponent::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	LOG(LogTouchInputsSystem, "%d Touch Pressed", FingerIndex)
	
	const TArray<UTouchInputComponent*> SortedAndFilteredComponents = SortByPriority(FilterByBounds(FilterByActive(TouchInputComponents), Location));

	LOG(LogTouchInputsSystem, "%d Touch Press Affect %d Touch Components", FingerIndex, SortedAndFilteredComponents.Num())

	int HighestTouchInputPriority = MAX_int32;
	
	for(UTouchInputComponent* Component : SortedAndFilteredComponents)
	{
		if(HighestTouchInputPriority > Component->Priority && HighestTouchInputPriority != MAX_int32)
		{
			return;
		}
		
		Component->OnEventTouchPressed(FingerIndex, Location);
		
		HighestTouchInputPriority = Component->Priority;
	}
}

void UTouchInputsComponent::OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	LOG(LogTouchInputsSystem, "%d Touch Moved", FingerIndex)
	
	for(UTouchInputComponent* Component : TouchInputComponents)
	{
		if(Component->IsFingerExists(FingerIndex))
		{
			Component->OnEventTouchMoved(FingerIndex, Location);
		}
	}
}

void UTouchInputsComponent::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	LOG(LogTouchInputsSystem, "%d Touch Released", FingerIndex)
	for(UTouchInputComponent* Component : TouchInputComponents)
	{
		if(Component->IsFingerExists(FingerIndex))
		{
			Component->OnEventTouchReleased(FingerIndex, Location);
		}
	}
}