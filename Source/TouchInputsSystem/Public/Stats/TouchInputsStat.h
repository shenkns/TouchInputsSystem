// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Interfaces/SerializationInterface.h"
#include "Stats/Stat.h"

#include "Libs/SerializationSystemLibrary.h"
#include "TouchInputsConfigurationObjects/TouchInputSaveObject.h"

#include "TouchInputsStat.generated.h"

class UTouchInputSlotData;
class UTouchInputSaveObject;

UCLASS()
class TOUCHINPUTSSYSTEM_API UTouchInputsStat : public UStat, public ISerializationInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(SkipSerialization, EditAnywhere, BlueprintReadOnly, Category = "TouchInputs")
	TMap<UTouchInputSlotData*, UTouchInputSaveObject*> InputsObjects;

private:

	UPROPERTY()
	TMap<UTouchInputSlotData*, FObjectSaveData> SaveData;

public:

	virtual bool PreConvertToSaveData_Implementation() override;
	virtual bool PostConvertFromSaveData_Implementation() override;

	template<typename T>
	T* GetSlotSave(UTouchInputSlotData* Slot) const;

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (CompactNodeTitle = "HasSlot"))
	bool HasSlotSave(UTouchInputSlotData* Slot) const;

	UFUNCTION(BlueprintPure, Category = "TouchInputs", meta = (DeterminesOutputType = "Class", CompactNodeTitle = "Slot"))
	UTouchInputSaveObject* GetSlotSave(UTouchInputSlotData* Slot, TSubclassOf<UTouchInputSaveObject> Class) const;
};

template <typename T>
T* UTouchInputsStat::GetSlotSave(UTouchInputSlotData* Slot) const
{
	if(!HasSlotSave(Slot)) return nullptr;

	UTouchInputSaveObject* const* SaveObject = InputsObjects.Find(Slot);

	return *SaveObject ? static_cast<T*>(*SaveObject) : nullptr;
}
