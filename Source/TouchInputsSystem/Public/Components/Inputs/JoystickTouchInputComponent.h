// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Components/Inputs/TouchInputComponent.h"
#include "JoystickTouchInputComponent.generated.h"

class UJoystickTouchInputDebugWidget;

UENUM(BlueprintType)
enum EJoystickAxis
{
	None = 0,
	X = 1,
	Y = 2,
	Both = 3
};

UENUM(BlueprintType)
enum EJoystickMobilityType
{
	Fixed = 1,
	Free = 0,
	Floating = 2
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoystickAxisChangedEvent, float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoystickLocationChanged, FVector, Location);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJoystickClamped);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FJoystickValuesChanged, FVector2D, Axes, FVector, Direction)
;

UCLASS()
class TOUCHINPUTSSYSTEM_API UJoystickTouchInputComponent : public UTouchInputComponent
{
	GENERATED_BODY()

public:
	
	UJoystickTouchInputComponent();

public:
	
	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Joystick|Delegates")
	FJoystickAxisChangedEvent OnJoystickXAxisChanged;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Joystick|Delegates")
	FJoystickAxisChangedEvent OnJoystickYAxisChanged;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Joystick|Delegates")
	FJoystickLocationChanged OnJoystickLocationChanged;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Joystick|Delegates")
	FJoystickLocationChanged OnPickerLocationChanged;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Joystick|Delegates")
	FJoystickClamped OnJoystickClamped;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Joystick|Delegates")
	FJoystickValuesChanged OnJoystickValuesChanged;

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	float MaxMagnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	float ClampingSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	bool bEnableFloating;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Joystick")
	TEnumAsByte<EJoystickAxis> LockedAxis;

protected:
	
	UPROPERTY()
	UJoystickTouchInputDebugWidget* JoystickDebugWidget;

	FVector PressLocation;
	FVector DragLocation;
	FVector Direction;
	float PreviousMagnitude;
	float Magnitude;
	float Angle;
	float JoystickOffset = 1.f;
	FVector2D Axes;
	bool bReleased = true;
	TEnumAsByte<ETouchIndex::Type> CurrentFingerIndex;
	bool bClamped = true;

	UPROPERTY()
	UUserWidget* JoystickWidget;

	bool bHideOnUnPossession;
	bool bUseWidgetSize;

	bool bUseFixedLocation;
	bool bUseFixedLocationPercentage;
	bool bRecalculateImmediately;
	bool bAdjustBoundsToFixedLocation;
	FVector2D FixedLocation;
	FVector2D FixedLocationPercentage;

protected:
	
	void UpdateAxesVector();
	bool UpdateDragLocationFixed();
	void UpdateTempVariables();
	void UpdateLockAxes();
	void UpdateFloating();

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location) override;
	virtual void OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location) override;

	bool IsJoystickPressed(TEnumAsByte<ETouchIndex::Type> FIngerIndex) const;
	FVector GetLocationFixed(FVector Location) const;
	bool IsFitsFixedLocation(FVector Location) const;

	void SetupJoystickWidget(TSubclassOf<UUserWidget> WidgetClass, FLinearColor Tint, bool bUseWidgetSize, bool bHideIfUnPossessed);
	void SetupFixedLocation(bool bIsEnabled, FVector2D Location, bool bIsInPercent, bool bImmediateRecalculation, bool bAdjustBoundsToLocation);

	virtual bool ValidateDebugWidget() override;
};
