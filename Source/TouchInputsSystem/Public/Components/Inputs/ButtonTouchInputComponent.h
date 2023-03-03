// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Components/Inputs/TouchInputComponent.h"

#include "ButtonTouchInputComponent.generated.h"

class UButtonTouchInputDebugWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDoubleTapEvent, FName, Name, FVector, Location, TEnumAsByte<ETouchIndex::Type>, FingerIndex, float, Interval);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FTwoFingersTapEvent, FVector, Location1, TEnumAsByte<ETouchIndex::Type>, FingerIndex1, FVector, Location2, TEnumAsByte<ETouchIndex::Type>, FingerIndex2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FTwoFingersDoubleTapEvent, FVector, Location1, TEnumAsByte<ETouchIndex::Type>, FingerIndex1, FVector, Location2, TEnumAsByte<ETouchIndex::Type>, FingerIndex2, float, Interval);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTwoFingersTapFailedEvent, float, Interval);


UCLASS()
class TOUCHINPUTSSYSTEM_API UButtonTouchInputComponent : public UTouchInputComponent
{
	GENERATED_BODY()

public:

	UButtonTouchInputComponent();

public:
	
	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Button|Delegates")
	FDoubleTapEvent OnDoubleTap;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Button|Delegates")
	FTwoFingersTapEvent OnTwoFingersTap;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Button|Delegates")
	FTwoFingersDoubleTapEvent OnTwoFingersDoubleTap;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Button|Delegates")
	FTwoFingersTapFailedEvent OnTwoFingersTapFailed;
	
	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Configuration")
	float DoubleTapInterval = 0.25;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Configuration")
	float TwoFingersTapInterval = 0.1;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Temp")
	bool bReleased = true;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Temp")
	TEnumAsByte<ETouchIndex::Type> CurrentFingerIndex;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Temp")
	FDateTime OneFingerTapTime;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Temp")
	bool bTwoFingerTap;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Temp")
	FDateTime TwoFingersTapTime;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Temp")
	FDateTime LastTapTime;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Temp")
	UButtonTouchInputDebugWidget* ButtonDebugWidget;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	virtual void OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location) override;

protected:

	virtual bool ValidateDebugWidget() override;
};
