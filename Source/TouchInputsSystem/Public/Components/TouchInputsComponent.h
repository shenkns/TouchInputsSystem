// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Components/ActorComponent.h"

#include "TouchInputsComponent.generated.h"

class UTouchInputSlotData;
class UTouchInputObject;
class UTouchInputComponent;

UCLASS(ClassGroup = (TouchInputs), meta = (BlueprintSpawnableComponent))
class TOUCHINPUTSSYSTEM_API UTouchInputsComponent : public UActorComponent
{
	GENERATED_BODY()

	UTouchInputsComponent();

protected:

	bool bPossessed;
	bool bBinded;

public:

	UFUNCTION(BlueprintPure, Category = "TouchInputs")
	static TArray<UTouchInputComponent*> FilterByActive(TArray<UTouchInputComponent*> Components);

	UFUNCTION(BlueprintPure, Category = "TouchInputs")
	static TArray<UTouchInputComponent*> FilterByBounds(TArray<UTouchInputComponent*> Components, FVector Location);

	UFUNCTION(BlueprintPure, Category = "TouchInputs")
	static TArray<UTouchInputComponent*> SortByPriority(TArray<UTouchInputComponent*> Components);

	UFUNCTION(BlueprintPure, Category = "TouchInputs")
	APlayerController* GetOwningPlayerController() const;

	UFUNCTION(BlueprintCallable)
	void PossessionUpdated();

protected:

	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void OnOwnerEndPlay(AActor* Actor , EEndPlayReason::Type EndPlayReason);

private:

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	
	void Init();

	void BindTouchEvents();
	void BindUnPossess();

	void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);
};
