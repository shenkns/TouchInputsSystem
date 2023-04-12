// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Components/Inputs/JoystickTouchInputComponent.h"

#include "LogSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Module/TouchInputsSystemModule.h"
#include "Widgets/JoystickTouchInputDebugWidget.h"

UJoystickTouchInputComponent::UJoystickTouchInputComponent()
{
	MaxFingersCount = 1;

	DebugWidgetClass = UJoystickTouchInputDebugWidget::StaticClass();
}

void UJoystickTouchInputComponent::UpdateAxesVector()
{
	if(LockedAxis != EJoystickAxis::X && LockedAxis != EJoystickAxis::Both)
	{
		const float DirectionSign = Direction.X > 0.f ? 1.f : -1.f;

		Axes.X = DirectionSign * FMath::Min(1.f, FMath::Abs(JoystickOffset * UKismetMathLibrary::DegSin(Angle) * FMath::Min(1.f, Magnitude / MaxMagnitude)));
	}
	else
	{
		Axes.X = 0.f;
	}

	if(LockedAxis != EJoystickAxis::Y && LockedAxis != EJoystickAxis::Both)
	{
		const float DirectionSign = Direction.Y > 0.f ? 1.f : -1.f;

		Axes.Y = DirectionSign * FMath::Min(1.f, FMath::Abs(JoystickOffset * UKismetMathLibrary::DegCos(Angle) * FMath::Min(1.f, Magnitude / MaxMagnitude)));
	}
	else
	{
		Axes.Y = 0.f;
	}
}

bool UJoystickTouchInputComponent::UpdateDragLocationFixed()
{
	if(bUseFixedLocation && LockedAxis != EJoystickAxis::None)
	{
		if(LockedAxis == EJoystickAxis::X) DragLocation = FVector(PressLocation.X, DragLocation.Y, 0.f);
		else if(LockedAxis == EJoystickAxis::Y) DragLocation = FVector(DragLocation.X, PressLocation.Y, 0.f);
		else DragLocation = PressLocation;

		return true;
	}

	return false;
}

void UJoystickTouchInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bReleased)
	{
		if(Axes.X != 0.f || Axes.Y != 0.f)
		{
			Axes = UKismetMathLibrary::Vector2DInterpTo(Axes, FVector2D(), DeltaTime, ClampingSpeed);

			OnJoystickXAxisChanged.Broadcast(Axes.X);
			OnJoystickYAxisChanged.Broadcast(Axes.Y);

			OnJoystickValuesChanged.Broadcast(Axes, Direction);

			LOG(LogTouchInputsSystem, "Axes Changed: %s", *FVector2D(Axes).ToString())
		}

		if(PressLocation.X != DragLocation.X || PressLocation.Y != DragLocation.Y)
		{
			DragLocation = UKismetMathLibrary::VInterpTo(DragLocation, PressLocation, DeltaTime, ClampingSpeed);

			OnPickerLocationChanged.Broadcast(DragLocation);

			LOG(LogTouchInputsSystem, "Picker Location Changed Delta: %s", *FVector2D(DragLocation).ToString())
		}

		if(!((Axes.X != 0.f || Axes.Y != 0.f) && (PressLocation.X != DragLocation.X || PressLocation.Y != DragLocation.Y)))
		{
			if(!bClamped)
			{
				OnJoystickClamped.Broadcast();

				LOG(LogTouchInputsSystem, "Joystick Clamped")
			}
		}
	}
	else
	{
		bClamped = false;

		UpdateAxesVector();

		OnJoystickXAxisChanged.Broadcast(Axes.X);
		OnJoystickYAxisChanged.Broadcast(Axes.Y);

		OnJoystickValuesChanged.Broadcast(Axes, Direction);

		LOG(LogTouchInputsSystem, "Joystick Values Changed: %s, Direction: %s, Angle: %f", *FVector2D(Axes).ToString(), *Direction.ToString(), Angle)
	}
}

void UJoystickTouchInputComponent::OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchPressed(FingerIndex, Location);

	if(!IsFingerExists(FingerIndex)) return;

	if(IsJoystickPressed(FingerIndex))
	{
		OnEventTouchReleased(FingerIndex, Location);
	}

	if(!IsFitsFixedLocation(Location)) return;

	PressLocation = GetLocationFixed(Location);
	CurrentFingerIndex = FingerIndex;
	DragLocation = Location;

	UpdateDragLocationFixed();

	bReleased = false;

	if(bUseFixedLocation && bRecalculateImmediately)
	{
		UpdateTempVariables();
	}

	OnTap.Broadcast(DragLocation, FingerIndex);
	OnJoystickLocationChanged.Broadcast(PressLocation);
	OnPickerLocationChanged.Broadcast(DragLocation);

	if(ValidateDebugWidget())
	{
		DebugWidget->BoundsOrigin = BoundsOrigin;
		DebugWidget->BoundsSize = BoundsSize;

		JoystickDebugWidget->JoystickLocation = FVector2D(PressLocation);
		JoystickDebugWidget->PickerLocation = FVector2D(DragLocation);
		JoystickDebugWidget->Radius = MaxMagnitude;
	}
}

void UJoystickTouchInputComponent::OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchMoved(FingerIndex, Location);

	if(!IsJoystickPressed(FingerIndex)) return;

	DragLocation = Location;

	UpdateTempVariables();
	UpdateFloating();
	UpdateLockAxes();

	OnJoystickLocationChanged.Broadcast(PressLocation);
	LOG(LogTouchInputsSystem, "Joystick Location Changed: %s", *PressLocation.ToString())
	
	OnPickerLocationChanged.Broadcast(DragLocation);
	LOG(LogTouchInputsSystem, "Picker Location Changed Delta: %s", *FVector2D(DragLocation).ToString())
	

	if(ValidateDebugWidget())
	{
		DebugWidget->BoundsOrigin = BoundsOrigin;
		DebugWidget->BoundsSize = BoundsSize;

		JoystickDebugWidget->JoystickLocation = FVector2D(PressLocation);
		JoystickDebugWidget->PickerLocation = FVector2D(DragLocation);
	}
}

void UJoystickTouchInputComponent::OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchReleased(FingerIndex, Location);

	if(!IsJoystickPressed(FingerIndex)) return;

	bReleased = true;
	Magnitude = 0.f;
	Angle = 0.f;

	OnTapRelease.Broadcast(Location, FingerIndex);

	if(IsValid(DebugWidget))
	{
		DebugWidget->RemoveFromParent();
		DebugWidget = nullptr;
		JoystickDebugWidget = nullptr;
	}
}

bool UJoystickTouchInputComponent::IsJoystickPressed(TEnumAsByte<ETouchIndex::Type> FIngerIndex) const
{
	return CurrentFingerIndex == FIngerIndex && !bReleased;
}

FVector UJoystickTouchInputComponent::GetLocationFixed(FVector Location) const
{
	return bUseFixedLocation ? FVector(FixedLocation, 0.f) : Location;
}

bool UJoystickTouchInputComponent::IsFitsFixedLocation(FVector Location) const
{
	return !bUseFixedLocation || (Location - FVector(FixedLocation, 0.f)).Size() < MaxMagnitude;
}

void UJoystickTouchInputComponent::SetupJoystickWidget(TSubclassOf<UUserWidget> WidgetClass, FLinearColor Tint, bool bWidgetSize, bool bHideUnPossessed)
{
	
}

void UJoystickTouchInputComponent::SetupFixedLocation(bool bIsEnabled, FVector2D Location, bool bIsInPercent, bool bImmediateRecalculation, bool bAdjustBoundsToLocation)
{
	
}

bool UJoystickTouchInputComponent::ValidateDebugWidget()
{
	if(Super::ValidateDebugWidget())
	{
		JoystickDebugWidget = Cast<UJoystickTouchInputDebugWidget>(DebugWidget);

		return IsValid(JoystickDebugWidget);
	}

	return false;
}

void UJoystickTouchInputComponent::UpdateTempVariables()
{
	PreviousMagnitude = Magnitude;
	Magnitude = (DragLocation - PressLocation).Size();
	Direction = (DragLocation - PressLocation).GetSafeNormal(0.f);
	Angle = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(ScreenUpVector,Direction));
}

void UJoystickTouchInputComponent::UpdateLockAxes()
{
	if(!UpdateDragLocationFixed() && bIsInBounds)
	{
		PressLocation.X = LockedAxis != EJoystickAxis::X && LockedAxis != EJoystickAxis::Both ? PressLocation.X : UKismetMathLibrary::FInterpTo(DragLocation.X, PressLocation.X, GetWorld()->GetDeltaSeconds(), 12.f);
		PressLocation.Y = LockedAxis != EJoystickAxis::Y && LockedAxis != EJoystickAxis::Both ? PressLocation.Y : UKismetMathLibrary::FInterpTo(DragLocation.Y, PressLocation.Y, GetWorld()->GetDeltaSeconds(), 12.f);

		LOG(LogTouchInputsSystem, "Lock Axes Updated")
	}
}

void UJoystickTouchInputComponent::UpdateFloating()
{
	const bool IsNotMaxMagnitude = Magnitude <= MaxMagnitude * JoystickOffset || (PreviousMagnitude >= Magnitude && PreviousMagnitude <= MaxMagnitude * JoystickOffset);
	const float LerpAlpha = (Magnitude - MaxMagnitude * JoystickOffset) / FMath::Max(0.01f, Magnitude);
	
	if(bEnableFloating && !bUseFixedLocation)
	{
		if(bUseBounds)
		{
			PressLocation = !(CheckBounds(DragLocation) && IsNotMaxMagnitude) ? FVector(CheckBoundsX(DragLocation) ? UKismetMathLibrary::VLerp(PressLocation, DragLocation, LerpAlpha).X : PressLocation.X, CheckBoundsY(DragLocation) ? UKismetMathLibrary::VLerp(PressLocation, DragLocation, LerpAlpha).Y : PressLocation.Y, 0.f) : PressLocation;
		}
		else
		{
			PressLocation = !IsNotMaxMagnitude ? UKismetMathLibrary::VLerp(PressLocation, DragLocation, LerpAlpha) : PressLocation;
		}
	}
	
	DragLocation = IsNotMaxMagnitude ? DragLocation : PressLocation + Direction * MaxMagnitude;

	LOG(LogTouchInputsSystem, "Floating Updated")
}
