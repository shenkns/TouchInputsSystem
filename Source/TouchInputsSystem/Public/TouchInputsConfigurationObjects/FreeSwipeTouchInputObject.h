// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "TouchInputsConfigurationObjects/TouchInputObject.h"
#include "FreeSwipeTouchInputObject.generated.h"

class UFreeSwipeTouchInputComponent;

UCLASS()
class TOUCHINPUTSSYSTEM_API UFreeSwipeTouchInputObject : public UTouchInputObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UFreeSwipeTouchInputComponent* FreeSwipeTouchInputComponent;

	// Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|FreeSwipe")
	float Speed = 0.15;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|FreeSwipe")
	bool bEnableInterpolation = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|FreeSwipe")
	float InterpolationSpeed = 16.f;

protected:

	virtual void SetTouchInputComponentEssentials() override;
};
