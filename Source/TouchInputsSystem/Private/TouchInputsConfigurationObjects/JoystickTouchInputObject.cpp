// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "TouchInputsConfigurationObjects/JoystickTouchInputObject.h"

#include "LogSystem.h"
#include "Module/TouchInputsSystemModule.h"

void UJoystickTouchInputObject::SetTouchInputComponentEssentials()
{
	Super::SetTouchInputComponentEssentials();

	JoystickTouchInputComponent = Cast<UJoystickTouchInputComponent>(TouchInputComponent);

	JoystickTouchInputComponent->MaxMagnitude = Size;
	JoystickTouchInputComponent->ClampingSpeed = ClampingSpeed;

	JoystickTouchInputComponent->bEnableFloating = JoystickMobility == EJoystickMobilityType::Floating;;

	JoystickTouchInputComponent->LockedAxis = LockedAxis;

	LOG(LogTouchInputsSystem, "Joystick Touch Input Component Essentials Set")
}

void UJoystickTouchInputObject::InitTouchInputComponent()
{
	Super::InitTouchInputComponent();

	JoystickTouchInputComponent->SetupJoystickWidget(nullptr, Tint, true, true);
	JoystickTouchInputComponent->SetupFixedLocation(JoystickMobility == EJoystickMobilityType::Fixed, BoundsOrigin, bBoundsPercentage, true, true);
}
