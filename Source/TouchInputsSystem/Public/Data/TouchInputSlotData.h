// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Data/Data.h"

#include "TouchInputSlotData.generated.h"

class UTouchInputTypeData;
class UTouchInputConfigWidget;

UCLASS()
class TOUCHINPUTSSYSTEM_API UTouchInputSlotData : public UData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInputs")
	UTouchInputTypeData* TouchInputType;
};
