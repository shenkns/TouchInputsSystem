// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Components/Inputs/FreeSwipeTouchInputComponent.h"

#include "Log.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Log/Details/LocalLogCategory.h"
#include "Module/TouchInputsSystemModule.h"
#include "Module/TouchInputsSystemSettings.h"
#include "TouchInputsConfigurationObjects/FreeSwipeTouchInputSaveObject.h"
#include "Widgets/TouchInputDebugWidget.h"

DEFINE_LOG_CATEGORY_LOCAL(LogTouchInputsSystem);

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

	LOG(Display, "Location Changed Delta: {}", *FVector2D(CurrentDelta).ToString());
}

void UFreeSwipeTouchInputComponent::OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchPressed(FingerIndex, Location);

	StartLocation = Location;
	PreviousLocation = Location;

	OnTap.Broadcast(Location, FingerIndex);

	LOG(Display, "Tap: Touch {} {}", (int)FingerIndex, Location);

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

	LOG(Display, "Location Changed Delta: {}", *FVector2D(CurrentDelta).ToString());

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

	LOG(Display, "Release: Touch %d %s", (int)FingerIndex, Location);

	const UTouchInputsSystemSettings* Settings = GetDefault<UTouchInputsSystemSettings>();
	if(Settings && Settings->bShowDebugShapes && DebugWidget)
	{
		DebugWidget->RemoveFromParent();
		DebugWidget = nullptr;
	}
}

void UFreeSwipeTouchInputComponent::RecalculateDelta(FVector CurrentLocation, FVector& CalculationTargetDelta, FVector& CalculationCurrentDelta) const
{
	CalculationTargetDelta = (CurrentLocation - PreviousLocation) * (Speed * GetWorld()->GetDeltaSeconds());

	CalculationCurrentDelta = UKismetMathLibrary::VInterpTo(CurrentDelta, TargetDelta, GetWorld()->GetDeltaSeconds(), InterpolationSpeed);
}

UTouchInputSaveObject* UFreeSwipeTouchInputComponent::CreateSaveObject()
{
	return NewObject<UFreeSwipeTouchInputSaveObject>(this);
}
