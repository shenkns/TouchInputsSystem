// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Widgets/ButtonTouchInputDebugWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

int32 UButtonTouchInputDebugWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext InContext = FPaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	for(const FVector2D& TapLocation : TapLocations)
	{
		DrawRadialShape(InContext, TapLocation / ViewportScale, 68.f / ViewportScale, 32, FColor::Red);

		UWidgetBlueprintLibrary::DrawText(InContext, bDoubleTap ? "2X TAP" : "TAP", TapLocation / ViewportScale - FVector2D(42.f / ViewportScale, 51.f / ViewportScale));
	}
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}
