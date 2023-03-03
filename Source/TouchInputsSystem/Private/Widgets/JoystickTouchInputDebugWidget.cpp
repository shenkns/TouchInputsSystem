// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Widgets/JoystickTouchInputDebugWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

int32 UJoystickTouchInputDebugWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext InContext = FPaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	DrawRadialShape(InContext, JoystickLocation / ViewportScale, Radius / ViewportScale, 32, FColor::Red);
	DrawRadialShape(InContext, PickerLocation / ViewportScale, Radius / 3.f / ViewportScale, 32, FColor::Red);

	UWidgetBlueprintLibrary::DrawLine(InContext, (JoystickLocation - FVector2D(1.f, 0.f) * Radius) / ViewportScale, (JoystickLocation + FVector2D(1.f, 0.f) * Radius) / ViewportScale, FColor::Red, false, 5.f);
	UWidgetBlueprintLibrary::DrawLine(InContext, (JoystickLocation - FVector2D(0.f, 1.f) * Radius) / ViewportScale, (JoystickLocation + FVector2D(0.f, 1.f) * Radius) / ViewportScale, FColor::Red, false, 5.f);
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
