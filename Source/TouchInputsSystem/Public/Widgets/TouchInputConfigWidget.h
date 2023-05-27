// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Blueprint/UserWidget.h"

#include "TouchInputConfigWidget.generated.h"

class UTouchInputSaveObject;
class UTouchInputSlotData;
class UTouchInputPresetSlot;

UCLASS()
class TOUCHINPUTSSYSTEM_API UTouchInputConfigWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "TouchInput")
	UTouchInputPresetSlot* PresetSlot;
	
	UPROPERTY(BlueprintReadOnly, Category = "TouchInput")
	UTouchInputSlotData* InputSlotData;

	UPROPERTY(BlueprintReadOnly, Category = "TouchInput")
	UTouchInputSaveObject* SaveObject;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TouchInput")
	void InitWithSlot(UTouchInputPresetSlot* NewPresetSlot, UTouchInputSlotData* NewSlot);

protected:

	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	UFUNCTION(BlueprintNativeEvent, Category = "TouchInput")
	void SaveDataToSlot();

	UFUNCTION(BlueprintNativeEvent, Category = "TouchInput")
	void LoadDataFromSlot();

	UFUNCTION(BlueprintCallable, Category = "TouchInput")
	void SetSize(FVector2D NewSize);

	UFUNCTION(BlueprintCallable, Category = "TouchInput")
	void SetOrigin(FVector2D NewOrigin);

private:

	FVector2D BoundsOrigin;
};
