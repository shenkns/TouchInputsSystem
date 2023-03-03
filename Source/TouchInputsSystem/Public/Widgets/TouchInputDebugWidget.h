// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Blueprint/UserWidget.h"
#include "TouchInputDebugWidget.generated.h"

UCLASS()
class TOUCHINPUTSSYSTEM_API UTouchInputDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button|Debug")
	FVector2D BoundsSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Button|Debug")
	FVector2D BoundsOrigin;

protected:
	
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void DrawBoundingBox(FPaintContext& InContext, FVector2D Location, FVector2D Size) const;
	void DrawRadialShape(FPaintContext& InContext, FVector2D Center, float Radius, int Segments, FLinearColor Color) const;
};
