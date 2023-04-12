// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "UObject/Object.h"

#include "TouchInputSaveObject.generated.h"

class UTouchInputSlotData;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class TOUCHINPUTSSYSTEM_API UTouchInputSaveObject : public UObject
{
	GENERATED_BODY()

protected:

	// Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput")
	bool bEnableDebugDrawings = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput")
	int Priority;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput")
	FVector2D BoundsOrigin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput")
	FVector2D BoundsSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput")
	bool bBoundsPercentage = true;
};
