// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "TouchInputsConfigurationObjects/TouchInputSaveObject.h"

#include "JoystickTouchInputSaveObject.generated.h"

UCLASS()
class TOUCHINPUTSSYSTEM_API UJoystickTouchInputSaveObject : public UTouchInputSaveObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	float MaxMagnitudeSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	bool bEnableFloating;
};
