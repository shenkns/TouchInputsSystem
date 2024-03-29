// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "TouchInputsConfigurationObjects/TouchInputSaveObject.h"

#include "ButtonTouchInputSaveObject.generated.h"

UCLASS()
class TOUCHINPUTSSYSTEM_API UButtonTouchInputSaveObject : public UTouchInputSaveObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TouchInput|Button|Background")
	UTexture2D* Background;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TouchInput|Button|Background")
	FLinearColor Tint;
};
