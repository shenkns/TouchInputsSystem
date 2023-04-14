// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Components/ActorComponent.h"

#include "InputCoreTypes.h"

#include "TouchInputComponent.generated.h"

class UTouchInputSaveObject;
class UTouchInputSlotData;
class UUserWidget;
class UTouchInputDebugWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewportSizeChangedEvent, FVector2D, PreviousSize, FVector2D, UpdatedSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNetworkConnectedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTapEvent, FVector, Location, TEnumAsByte<ETouchIndex::Type>, FingerIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTapMoveEvent, FVector, Location, TEnumAsByte<ETouchIndex::Type>, FingerIndex);

UCLASS(Abstract, BlueprintType, Blueprintable, ClassGroup = (TouchInputs), meta = (BlueprintSpawnableComponent))
class TOUCHINPUTSSYSTEM_API UTouchInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UTouchInputComponent();

public:

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Delegates")
	FTapEvent OnTap;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Delegates")
	FTapEvent OnTapRelease;
	
	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Delegates")
	FTapMoveEvent OnTapMove;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Delegates")
	FViewportSizeChangedEvent OnViewportSizeChanged;

	UPROPERTY(BlueprintAssignable, Category = "TouchInput|Delegates")
	FNetworkConnectedEvent OnNetworkConnected;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput")
	UTouchInputSlotData* Slot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput")
	int Priority;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Bounds")
	bool bUseBounds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	FVector2D BoundsOriginSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	FVector2D BoundsSizeSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds", EditConditionHides))
	bool bBoundsPercentage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Bounds", meta = (EditCondition = "bUseBounds && bBoundsPercentage", EditConditionHides))
	bool bRectangular;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Background")
	TSubclassOf<UUserWidget> BackgroundWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Save")
	bool bSaveInputData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Save")
	int ConfigWidgetZOrder;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|Debug")
	bool bEnableDebugDrawing;

	FVector2D BoundsOrigin;
	FVector2D BoundsSize;
	FVector2D BoundsOriginPercentage;
	FVector2D BoundsSizePercentage;
	
	int MaxFingersCount;

	bool bCheckNetworkConnection = false;
	
	FVector2D ActualViewportSize;

	bool bIsInBounds;

	bool bIsOwnerPawnOrController;

	bool bIsPossessed;

	FVector2D TempViewportSize;

	UPROPERTY()
	UTouchInputDebugWidget* DebugWidget;

	TMap<TEnumAsByte<ETouchIndex::Type>, FVector> Fingers;

	const FVector ScreenUpVector = FVector(0.f, -1.f, 0.f);
	const FVector ScreenRightVector = FVector(1.f, 0.f, 0.f);
	
	UPROPERTY()
	UUserWidget* BackgroundWidget;

	bool bUseBackgroundWidgetSizeAndOrigin;

	FVector2D BackgroundWidgetForceSize;

	bool bBackgroundWidgetSizeInPercent;

	bool bBackgroundWidgetRectangularSize;

	FVector2D BackgroundWidgetForceOrigin;

	bool bBackgroundWidgetOriginInPercent;

protected:

	UPROPERTY()
	TSubclassOf<UTouchInputDebugWidget> DebugWidgetClass;
	
public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CheckViewportSizeChanged();
	void CheckPawnPossessedByPlayer();
	bool IsBoundingEnabled() const;
	bool CheckBoundsX(FVector Location) const;
	bool CheckBoundsY(FVector Location) const;
	bool CheckBounds(FVector Location) const;

	void ViewportChangingCheck();
	void NetworkConnectionCheck();

	void OnPawnPossessed(AController* NewController);
	void OnPawnUnPossessed(AController* OldController);
	
	virtual void OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);
	virtual void OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location);
	virtual void OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

	bool AddFingerLocation(FVector Location, TEnumAsByte<ETouchIndex::Type> FingerIndex);
	void RemoveFingerLocation(TEnumAsByte<ETouchIndex::Type> FingerIndex);
	void UpdateFingerLocation(FVector NewLocation, TEnumAsByte<ETouchIndex::Type> FingerIndex);
	TEnumAsByte<ETouchIndex::Type> GetFingerIndexAtIndex(int Index) const;
	FVector GetFingerLocationAtIndex(int Index) const;
	
	virtual void UpdateBoundsInPercent(bool bIsViewportChanged);
	void UpdateBackgroundWidget();
	void UpdateViewportSize() {};

	FVector2D GetScreenSizeBySizeInPercent(FVector2D PercentSize) const;

	bool IsPlayerControllerLoaded() const;
	bool IsFingerExists(TEnumAsByte<ETouchIndex::Type> FingerIndex) const;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	virtual void SetupBounds();
	void SetupBackgroundsByBounds(UTexture2D* Background, FLinearColor Tint);
	void SetupBackgroundWidget(UTexture2D* Background, FLinearColor Tint, bool bUseBoundsSizeAndOrigin, FVector2D Origin, bool bOriginInPercent, FVector2D Size, bool bSizeInPercent, bool bIsRectangular);

	UFUNCTION(BlueprintPure, Category = "TouchInput")
	int GetTouchInputPriority() const { return Priority; }

	virtual void BeginDestroy() override;

	void Init();
	
protected:

	virtual bool ValidateDebugWidget();

	virtual void DestroyComponent(bool bPromoteChildren) override;
	
	bool LoadInputData();
	void SaveInputData();
	virtual void AddSaveDataToObject(UTouchInputSaveObject* SaveObject);
	virtual void LoadDataFromSaveObject(UTouchInputSaveObject* SaveObject);
	virtual UTouchInputSaveObject* CreateSaveObject();
};

