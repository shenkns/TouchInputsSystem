// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Data/Data.h"

#include "TouchInputTypeData.generated.h"

class UTouchInputConfigWidget;

UCLASS()
class TOUCHINPUTSSYSTEM_API UTouchInputTypeData : public UData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInputs")
	TSubclassOf<UTouchInputConfigWidget> ConfigWidgetClass;
};
