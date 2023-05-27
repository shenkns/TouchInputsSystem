// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "UObject/Interface.h"

#include "TouchInputWidgetInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTouchInputWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class TOUCHINPUTSSYSTEM_API ITouchInputWidgetInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnPickerLocationChanged(FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnJoystickLocationChanged(FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnJoystickClamped();

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnJoystickPressed(FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnJoystickReleased(FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnAxesChanged(float AxisX, float AxisY, FVector Direction);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnXAxisChanged(float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnYAxisChanged(float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	float GetMaxSize();

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void SetMaxSize(float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void OnViewportSizeChanged(FVector2D PreviousSize, FVector2D NewSize);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void SetSize(FVector2D Size);

	UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
	void SetBackground(UTexture2D* Background);
	
    UFUNCTION(BlueprintImplementableEvent, Category = "TouchInput|Widget")
    void SetOwningInputComponent(UTouchInputComponent* TouchInputComponent);
};
