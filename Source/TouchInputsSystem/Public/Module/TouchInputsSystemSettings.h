// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2023

#pragma once

#include "UObject/Object.h"

#include "TouchInputsSystemSettings.generated.h"

UCLASS(Config=Game, DefaultConfig)
class TOUCHINPUTSSYSTEM_API UTouchInputsSystemSettings : public UObject
{
	GENERATED_BODY()

public:

	// Debug
	UPROPERTY(Config, EditDefaultsOnly, Category = "Debug")
	bool bShowDebugMessages;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Debug")
	bool bShowDebugShapes;
};
