// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Components/TouchInputsComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Inputs/TouchInputComponent.h"
#include "Module/TouchInputsSystemModule.h"
#include "LogSystem.h"
#include "Engine/ActorChannel.h"

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
		return ComponentA.GetTouchInputPriority() > ComponentB.GetTouchInputPriority();
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
		
		return nullptr;
	}
	
	if(APlayerController* OwningController = GetOwner<APlayerController>())
	{
		return OwningController;
	}
	
	return nullptr;
}

void UTouchInputsComponent::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if(NewPawn == GetOwner()) return;

	bPossessed = false;
	
	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);

	for(UTouchInputComponent* TouchInput : Components)
	{
		TouchInput->K2_DestroyComponent(TouchInput);
	}

	if(!NewPawn) return;
	
	if(APlayerController* Controller = NewPawn->GetController<APlayerController>())
	{
		Controller->OnPossessedPawnChanged.RemoveDynamic(this, &UTouchInputsComponent::OnPawnChanged);
	}
}

void UTouchInputsComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwningPlayerController())
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
	BindTouchEvents();

	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);

	for(UTouchInputComponent* TouchInputComponent : Components)
	{
		TouchInputComponent->Init();
	}
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
	
	LOG(LogTouchInputsSystem, "%s Touch Events Binded", *GetName())
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

	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);
	TArray<UTouchInputComponent*> SortedAndFilteredComponents = SortByPriority(FilterByBounds(FilterByActive(Components), Location));

	LOG(LogTouchInputsSystem, "%d Touch Press Affect %d Touch Components", FingerIndex, SortedAndFilteredComponents.Num())

	int HighestTouchInputPriority = MAX_int32;
	
	for(UTouchInputComponent* Component : SortedAndFilteredComponents)
	{
		if(HighestTouchInputPriority > Component->GetTouchInputPriority() && HighestTouchInputPriority != MAX_int32)
		{
			return;
		}
		
		Component->OnEventTouchPressed(FingerIndex, Location);
		
		HighestTouchInputPriority = Component->GetTouchInputPriority();
	}
}

void UTouchInputsComponent::OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	LOG(LogTouchInputsSystem, "%d Touch Moved", FingerIndex)

	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);
	
	for(UTouchInputComponent* Component : Components)
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
	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);
	
	for(UTouchInputComponent* Component : Components)
	{
		if(Component->IsFingerExists(FingerIndex))
		{
			Component->OnEventTouchReleased(FingerIndex, Location);
		}
	}
}