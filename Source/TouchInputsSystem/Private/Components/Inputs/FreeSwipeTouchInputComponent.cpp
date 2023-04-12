// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Components/Inputs/FreeSwipeTouchInputComponent.h"

#include "LogSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Module/TouchInputsSystemModule.h"
#include "Widgets/TouchInputDebugWidget.h"

UFreeSwipeTouchInputComponent::UFreeSwipeTouchInputComponent()
{
	MaxFingersCount = 1;

	bEnableInterpolation = true;
	Speed = 0.15;
	InterpolationSpeed = 16.f;
}

void UFreeSwipeTouchInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(UKismetSystemLibrary::IsDedicatedServer(this)) return;
	
	if(!bEnableInterpolation || !CurrentDelta.Equals(TargetDelta, 0.01)) return;

	CurrentDelta = UKismetMathLibrary::VInterpTo(CurrentDelta, TargetDelta, DeltaTime, InterpolationSpeed);

	OnDeltaXChanged.Broadcast(CurrentDelta.X);
	OnDeltaYChanged.Broadcast(CurrentDelta.Y);

	OnDeltaLocationChanged.Broadcast(FVector2D(CurrentDelta));

	LOG(LogTouchInputsSystem, "Location Changed Delta: %s", *FVector2D(CurrentDelta).ToString())
}

void UFreeSwipeTouchInputComponent::OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchPressed(FingerIndex, Location);

	StartLocation = Location;
	PreviousLocation = Location;

	OnTap.Broadcast(Location, FingerIndex);

	LOG(LogTouchInputsSystem, "Tap: Touch %d %s", FingerIndex, *Location.ToString())

	if(ValidateDebugWidget())
	{
		DebugWidget->BoundsOrigin = BoundsOrigin;
		DebugWidget->BoundsSize = BoundsSize;
	}
}

void UFreeSwipeTouchInputComponent::OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchMoved(FingerIndex, Location);

	if(bEnableInterpolation)
	{
		RecalculateDelta(Location, TargetDelta, CurrentDelta);
	}
	else
	{
		TargetDelta = Location;
		CurrentDelta = Location;
	}

	OnDeltaXChanged.Broadcast(CurrentDelta.X);
	OnDeltaYChanged.Broadcast(CurrentDelta.Y);

	OnDeltaLocationChanged.Broadcast(FVector2D(CurrentDelta));

	LOG(LogTouchInputsSystem, "Location Changed Delta: %s", *FVector2D(CurrentDelta).ToString())

	PreviousLocation = Location;

	if(ValidateDebugWidget())
	{
		DebugWidget->BoundsOrigin = BoundsOrigin;
		DebugWidget->BoundsSize = BoundsSize;
	}
}

void UFreeSwipeTouchInputComponent::OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchReleased(FingerIndex, Location);

	StartLocation = FVector(-1.f);

	if(bEnableInterpolation)
	{
		RecalculateDelta(Location, TargetDelta, CurrentDelta);
	}

	OnTapRelease.Broadcast(Location, FingerIndex);

	LOG(LogTouchInputsSystem, "Release: Touch %d %s", FingerIndex, *Location.ToString())

	if(bEnableDebugDrawing && DebugWidget)
	{
		DebugWidget->RemoveFromParent();
		DebugWidget = nullptr;
	}
}

void UFreeSwipeTouchInputComponent::RecalculateDelta(FVector CurrentLocation, FVector& CalculationTargetDelta, FVector& CalculationCurrentDelta) const
{
	CalculationTargetDelta = (CurrentLocation - PreviousLocation) * Speed;

	CalculationCurrentDelta = UKismetMathLibrary::VInterpTo(CurrentDelta, TargetDelta, GetWorld()->GetDeltaSeconds(), InterpolationSpeed);
}
