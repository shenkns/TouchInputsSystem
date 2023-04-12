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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	FVector2D BoundsOriginSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	FVector2D BoundsSizeSetup;
};
