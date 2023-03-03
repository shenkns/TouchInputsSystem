// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "UObject/Object.h"

#include "TouchInputObject.generated.h"

class AActor;
class UTouchInputComponent;
class UTouchInputSlotData;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class TOUCHINPUTSSYSTEM_API UTouchInputObject : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UTouchInputComponent* TouchInputComponent;

	UPROPERTY()
	UTouchInputSlotData* Slot;

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

public:

	UTouchInputComponent* AddTouchInputComponent(AActor* Owner, UTouchInputSlotData* InputSlot);

protected:

	virtual void SetTouchInputComponentEssentials();
	virtual void InitTouchInputComponent();
};
