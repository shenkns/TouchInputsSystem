// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "TouchInputsConfigurationObjects/TouchInputObject.h"
#include "Components/Inputs/JoystickTouchInputComponent.h"
#include "JoystickTouchInputObject.generated.h"

UCLASS()
class TOUCHINPUTSSYSTEM_API UJoystickTouchInputObject : public UTouchInputObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UJoystickTouchInputComponent* JoystickTouchInputComponent;

	// Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	bool bAnimated = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	TEnumAsByte<EJoystickMobilityType> JoystickMobility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	float Size = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	float ClampingSpeed = 12.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	TEnumAsByte<EJoystickAxis> LockedAxis;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	FLinearColor Tint = FColor::White;

protected:

	virtual void SetTouchInputComponentEssentials() override;
	virtual void InitTouchInputComponent() override;
};
