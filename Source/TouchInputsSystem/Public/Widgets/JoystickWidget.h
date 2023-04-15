// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Blueprint/UserWidget.h"

#include "JoystickWidget.generated.h"

class UJoystickTouchInputComponent;
UCLASS()
class TOUCHINPUTSSYSTEM_API UJoystickWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "TouchInput|Joystick")
	UJoystickTouchInputComponent* Joystick;
	
public:

	UFUNCTION(BlueprintNativeEvent, Category = "TouchInput|Joystick")
	void InitJoystickWidget(UJoystickTouchInputComponent* Component);
};
