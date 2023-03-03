// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Components/ActorComponent.h"

#include "InputCoreTypes.h"

#include "TouchInputComponent.generated.h"

class UUserWidget;
class UTouchInputDebugWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FViewportSizeChangedEvent, FVector2D, PreviousSize, FVector2D, UpdatedSize);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNetworkConnectedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTapEvent, FName, Name, FVector, Location, TEnumAsByte<ETouchIndex::Type>, FingerIndex);
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

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Bounds")
	bool bUseBounds = true;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Bounds")
	FVector2D BoundsOrigin;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Bounds")
	FVector2D BoundsSize;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Bounds")
	bool bBoundsPercentage = false;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Bounds")
	FVector2D BoundsOriginPercentage;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Bounds")
	FVector2D BoundsSizePercentage;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Bounds")
	bool bRectangular = false;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Configuration")
	FName Name;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Configuration")
	int Priority;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Configuration")
	int MaxFingersCount;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Configuration")
	bool bCheckNetworkConnection = false;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Configuration")
	bool bEnableDebugDrawing = true;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Temp")
	FVector2D ActualViewportSize;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Temp")
	bool bIsInBounds;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Temp")
	bool bIsOwnerPawnOrController;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Temp")
	bool bIsPossessed;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Temp")
	FVector2D TempViewportSize;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|Temp")
	UTouchInputDebugWidget* DebugWidget;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|FingersLocations")
	TMap<TEnumAsByte<ETouchIndex::Type>, FVector> Fingers;

	const FVector ScreenUpVector = FVector(0.f, -1.f, 0.f);
	const FVector ScreenRightVector = FVector(1.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TouchInput|BackgroundWidget")
	UUserWidget* BackgroundWidget;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|BackgroundWidget")
	bool bUseBackgroundWidgetSizeAndOrigin;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|BackgroundWidget")
	FVector2D BackgroundWidgetForceSize;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|BackgroundWidget")
	bool bBackgroundWidgetSizeInPercent;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|BackgroundWidget")
	bool bBackgroundWidgetRectangularSize;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|BackgroundWidget")
	FVector2D BackgroundWidgetForceOrigin;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|BackgroundWidget")
	bool bBackgroundWidgetOriginInPercent;

	UPROPERTY(EditDefaultsOnly, Category = "TouchInput|BackgroundWidget")
	TSubclassOf<UUserWidget> BackgroundWidgetClass;

protected:

	UPROPERTY()
	TSubclassOf<UTouchInputDebugWidget> DebugWidgetClass;
	
public:
	
	virtual void BeginPlay() override;
	
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
	
	void UpdateBoundsInPercent(bool bIsViewportChanged);
	void UpdateBackgroundWidget();
	void UpdateViewportSize() {};

	FVector2D GetScreenSizeBySizeInPercent(FVector2D PercentSize) const;

	bool IsPlayerControllerLoaded() const;
	bool IsFingerExists(TEnumAsByte<ETouchIndex::Type> FingerIndex) const;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	void SetupBounds(bool bIsEnabled, FVector2D NewBoundsOrigin, FVector2D NewBoundsSize, bool bIsInPercent, bool bIsRectangular);
	void SetupBackgroundsByBounds(UTexture2D* Background, FLinearColor Tint);
	void SetupBackgroundWidget(UTexture2D* Background, FLinearColor Tint, bool bUseBoundsSizeAndOrigin, FVector2D Origin, bool bOriginInPercent, FVector2D Size, bool bSizeInPercent, bool bIsRectangular);

protected:

	virtual bool ValidateDebugWidget();

	virtual void DestroyComponent(bool bPromoteChildren) override;
};
