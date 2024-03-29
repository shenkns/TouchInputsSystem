// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Components/Inputs/TouchInputComponent.h"
#include "FreeSwipeTouchInputComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeltaAxisChangedEvent, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeltaLocationChangedEvent, FVector2D, Value);

UCLASS()
class TOUCHINPUTSSYSTEM_API UFreeSwipeTouchInputComponent : public UTouchInputComponent
{
	GENERATED_BODY()

public:

	UFreeSwipeTouchInputComponent();

public:

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|FreeSwipe|Delegates")
	FDeltaAxisChangedEvent OnDeltaXChanged;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|FreeSwipe|Delegates")
	FDeltaAxisChangedEvent OnDeltaYChanged;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|FreeSwipe|Delegates")
	FDeltaLocationChangedEvent OnDeltaLocationChanged;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|FreeSwipe")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|FreeSwipe")
	bool bEnableInterpolation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|FreeSwipe")
	float InterpolationSpeed;

protected:

	FVector PreviousLocation;

	FVector CurrentDelta;

	FVector TargetDelta;

	FVector StartLocation;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location) override;

	void RecalculateDelta(FVector CurrentLocation, FVector& TargetDelta, FVector& CurrentDelta) const;

protected:

	virtual UTouchInputSaveObject* CreateSaveObject() override;
};
