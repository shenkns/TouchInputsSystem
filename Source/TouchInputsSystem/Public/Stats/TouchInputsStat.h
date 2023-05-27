// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2023.

#pragma once

#include "Data/TouchInputSlotData.h"
#include "Interfaces/SerializationInterface.h"
#include "Stats/Stat.h"

#include "Libs/SerializationSystemLibrary.h"
#include "TouchInputsConfigurationObjects/TouchInputSaveObject.h"

#include "TouchInputsStat.generated.h"

class UTouchInputSlotData;
class UTouchInputSaveObject;
class UTouchInputPresetSlot;

USTRUCT(BlueprintType)
struct FTouchInputPreset
{
	GENERATED_BODY()

	UPROPERTY(Instanced, SkipSerialization, EditAnywhere, BlueprintReadWrite)
	TMap<UTouchInputSlotData*, UTouchInputSaveObject*> InputObjects;
};

USTRUCT(BlueprintType)
struct FTouchInputPresetSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FObjectSaveData> InputObjectsSaveData;
};

UCLASS()
class TOUCHINPUTSSYSTEM_API UTouchInputsStat : public UStat, public ISerializationInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(SkipSerialization, EditAnywhere, BlueprintReadOnly, Category = "TouchInputs")
	TMap<UTouchInputPresetSlot*, FTouchInputPreset> InputPresets;

private:

	UPROPERTY()
	TMap<FName, FTouchInputPresetSaveData> SaveData;

public:

	virtual bool PreConvertToSaveData_Implementation() override;
	virtual bool PostConvertFromSaveData_Implementation() override;

	template<typename T>
	T* GetSlotSave(UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot) const;

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (CompactNodeTitle = "Presets"))
	TMap<UTouchInputPresetSlot*, FTouchInputPreset> GetPresets() const { return InputPresets; }

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (CompactNodeTitle = "HasPreset"))
	bool HasPreset(UTouchInputPresetSlot* Preset) const;

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (CompactNodeTitle = "Preset"))
	FTouchInputPreset GetPreset(UTouchInputPresetSlot* Preset) const;

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (CompactNodeTitle = "Slots"))
	TMap<UTouchInputSlotData*, UTouchInputSaveObject*> GetSlots(UTouchInputPresetSlot* Preset) const;

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (CompactNodeTitle = "HasSlot"))
	bool HasSlotSave(UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot) const;

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (DeterminesOutputType = "Class", CompactNodeTitle = "Slot"))
	UTouchInputSaveObject* GetSlotSave(UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot, TSubclassOf<UTouchInputSaveObject> Class) const;

	UFUNCTION(BlueprintCallable, Category = "TouchInputs")
	void SaveToSlot(UTouchInputSaveObject* InputObject, UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot);
};

template <typename T>
T* UTouchInputsStat::GetSlotSave(UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot) const
{
	if(!HasSlotSave(Preset, Slot)) return nullptr;

	UTouchInputSaveObject* const* SaveObject = GetPreset(Preset).InputObjects.Find(Slot);

	return *SaveObject ? static_cast<T*>(*SaveObject) : nullptr;
}
