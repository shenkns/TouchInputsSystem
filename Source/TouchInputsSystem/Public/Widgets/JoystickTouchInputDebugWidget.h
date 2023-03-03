// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Widgets/TouchInputDebugWidget.h"
#include "Components/Inputs/JoystickTouchInputComponent.h"
#include "JoystickTouchInputDebugWidget.generated.h"

UCLASS()
class TOUCHINPUTSSYSTEM_API UJoystickTouchInputDebugWidget : public UTouchInputDebugWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	FVector2D JoystickLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	FVector2D PickerLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	float Radius;

protected:

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
