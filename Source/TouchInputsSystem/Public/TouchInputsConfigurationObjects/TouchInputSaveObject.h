// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "UObject/Object.h"

#include "TouchInputSaveObject.generated.h"

class UTouchInputSlotData;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class TOUCHINPUTSSYSTEM_API UTouchInputSaveObject : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	FVector2D BoundsOriginSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	FVector2D BoundsSizeSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	bool bRectangular;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	bool bBoundsPercentage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TouchInput")
	int ConfigWidgetZOrder;
};
