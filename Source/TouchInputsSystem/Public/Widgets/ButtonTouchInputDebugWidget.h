// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Widgets/TouchInputDebugWidget.h"
#include "ButtonTouchInputDebugWidget.generated.h"

UCLASS()
class TOUCHINPUTSSYSTEM_API UButtonTouchInputDebugWidget : public UTouchInputDebugWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button|Debug")
	bool bDoubleTap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button|Debug")
	FVector2D Location;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button|Debug")
	TArray<FVector2D> TapLocations;

protected:

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
