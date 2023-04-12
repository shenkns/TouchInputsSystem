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
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION(Client, Reliable)
	void ClientBeginPlay();

private:
	
	void Init();

	void BindTouchEvents();
	void BindUnPossess();

	void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMoved(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);
};
