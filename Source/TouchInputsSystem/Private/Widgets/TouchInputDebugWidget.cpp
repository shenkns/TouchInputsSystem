// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Widgets/TouchInputDebugWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

int32 UTouchInputDebugWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext InContext = FPaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	DrawBoundingBox(InContext, BoundsOrigin, BoundsSize);
	
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void UTouchInputDebugWidget::DrawBoundingBox(FPaintContext& InContext, FVector2D Location, FVector2D Size) const
{
	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	Location += FVector2D(1.f, 0.f);
	
	const FVector2D SizeXAxis = FVector2D(Size.X, 0.f) - FVector2D(2.f, 0.f);
	const FVector2D SizeYAxis = FVector2D(0.f, Size.Y) - FVector2D(0.f, 2.f);

	const TArray<FVector2D> Points = { Location / ViewportScale, (Location + SizeXAxis) / ViewportScale, (Location + SizeXAxis + SizeYAxis) / ViewportScale, (Location + SizeYAxis) / ViewportScale, Location / ViewportScale };
	
	UWidgetBlueprintLibrary::DrawLines(InContext, Points, FColor::Red, false, 5.f);
}

void UTouchInputDebugWidget::DrawRadialShape(FPaintContext& InContext, FVector2D Center, float Radius, int Segments, FLinearColor Color) const
{
	const float AngleStep = PI * 2 / FMath::Max(3, Segments);

	float Angle = 0.f;
	
	for(int i = 0; i < FMath::Max(3, Segments); i++)
	{
		const FVector2D Start = Center + (FMath::Cos(Angle) * FVector2D(1.f, 0.f) + FMath::Sin(Angle) * FVector2D(0.f, 1.f)) * Radius;
		const FVector2D End = Center + (FMath::Cos(Angle + AngleStep) * FVector2D(1.f, 0.f) + FMath::Sin(Angle + AngleStep) * FVector2D(0.f, 1.f)) * Radius;
		
		UWidgetBlueprintLibrary::DrawLine(InContext, Start, End, Color, false, 5.f);

		Angle += AngleStep;
	}
}
