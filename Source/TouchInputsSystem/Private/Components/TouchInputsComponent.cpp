// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Components/TouchInputsComponent.h"

#include "Log.h"
#include "Components/Inputs/TouchInputComponent.h"
#include "Log/Details/LocalLogCategory.h"
#include "Module/TouchInputsSystemModule.h"

DEFINE_LOG_CATEGORY_LOCAL(LogTouchInputsSystem);

UTouchInputsComponent::UTouchInputsComponent()
{
	SetIsReplicatedByDefault(false);
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
	if(GetOwner<APlayerController>()) return;
	if(NewPawn == GetOwner()) return;

	bPossessed = false;
	
	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);

	for(UTouchInputComponent* TouchInput : Components)
	{
		TouchInput->Deactivate();
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
	if(bPossessed) return;
	
	bPossessed = true;
		
	Init();
}

void UTouchInputsComponent::Init()
{
	BindTouchEvents();

	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);

	for(UTouchInputComponent* TouchInputComponent : Components)
	{
		if(IsValid(TouchInputComponent))
		{
			TouchInputComponent->Init();
		}
	}
}

void UTouchInputsComponent::BindTouchEvents()
{
	if(bBinded) return;
	
	const APlayerController* OwningPlayerController = GetOwningPlayerController();

	if(!OwningPlayerController || !OwningPlayerController->InputComponent) return;
	
	OwningPlayerController->InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &UTouchInputsComponent::OnTouchPressed);
	LOG(Display, "Touch Press Binded");
	
	OwningPlayerController->InputComponent->BindTouch(EInputEvent::IE_Released, this, &UTouchInputsComponent::OnTouchReleased);
	LOG(Display, "Touch Release Binded");
	
	OwningPlayerController->InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &UTouchInputsComponent::OnTouchMoved);
	LOG(Display, "Touch Move Binded");

	bBinded = true;
	
	LOG(Display, "{} Touch Events Binded", this);
}

void UTouchInputsComponent::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	LOG(Display, "{} Touch Pressed", (int)FingerIndex);

	TArray<UTouchInputComponent*> Components;
	GetOwner()->GetComponents<UTouchInputComponent>(Components);
	TArray<UTouchInputComponent*> SortedAndFilteredComponents = SortByPriority(FilterByBounds(FilterByActive(Components), Location));

	LOG(Display, "{} Touch Press Affect {} Touch Components", (int)FingerIndex, SortedAndFilteredComponents.Num());

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
	LOG(Display, "{} Touch Moved", (int)FingerIndex);

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
	LOG(Display, "{} Touch Released", (int)FingerIndex);
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