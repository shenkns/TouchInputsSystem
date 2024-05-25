// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Components/Inputs/TouchInputComponent.h"

#include "ButtonTouchInputComponent.generated.h"

class UButtonTouchInputDebugWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDoubleTapEvent, FVector, Location, TEnumAsByte<ETouchIndex::Type>, FingerIndex, float, Interval);
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

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button")
	float DoubleTapInterval;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button")
	float TwoFingersTapInterval;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Background")
	UTexture2D* BackgroundTexture;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Button|Background")
	FLinearColor Tint;
	
	bool bReleased = true;

	TEnumAsByte<ETouchIndex::Type> CurrentFingerIndex;

	FDateTime OneFingerTapTime;

	bool bTwoFingerTap;

	FDateTime TwoFingersTapTime;

	FDateTime LastTapTime;

	UPROPERTY()
	UButtonTouchInputDebugWidget* ButtonDebugWidget;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location) override;

	virtual void SetupBounds() override;

protected:

	virtual bool ValidateDebugWidget() override;

	virtual UTouchInputSaveObject* CreateSaveObject() override;

	virtual void AddSaveDataToObject(UTouchInputSaveObject* SaveObject) override;
	virtual void LoadDataFromSaveObject(UTouchInputSaveObject* SaveObject) override;
};
