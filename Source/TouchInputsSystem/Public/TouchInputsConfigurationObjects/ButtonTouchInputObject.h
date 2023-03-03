// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "TouchInputsConfigurationObjects/TouchInputObject.h"
#include "ButtonTouchInputObject.generated.h"

class UTexture2D;
class UButtonTouchInputComponent;

UCLASS()
class TOUCHINPUTSSYSTEM_API UButtonTouchInputObject : public UTouchInputObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UButtonTouchInputComponent* ButtonTouchInputComponent;

	// Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button")
	UTexture2D* ButtonTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button")
	FLinearColor Tint = FColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button")
	float DoubleTapInterval = 0.25;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button")
	float TwoFingersTapInterval = 0.1;

protected:

	virtual void SetTouchInputComponentEssentials() override;
	virtual void InitTouchInputComponent() override;
};
