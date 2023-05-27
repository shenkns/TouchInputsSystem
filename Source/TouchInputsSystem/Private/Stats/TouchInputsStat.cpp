// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2023.


#include "Stats/TouchInputsStat.h"

#include "ManagersSystem.h"
#include "Data/TouchInputPresetSlot.h"
#include "Managers/DataManager.h"
#include "Managers/StatsManager.h"

bool UTouchInputsStat::PreConvertToSaveData_Implementation()
{
	SaveData.Empty();
	
	for(const TTuple<UTouchInputPresetSlot*, FTouchInputPreset>& PresetPair : InputPresets)
	{
		if(!PresetPair.Key) continue;;
		
		FTouchInputPresetSaveData PresetSaveData;

		for(const TTuple<UTouchInputSlotData*, UTouchInputSaveObject*> Pair : PresetPair.Value.InputObjects)
		{
			if(!Pair.Key || !Pair.Value) continue;

			FObjectSaveData ObjectSaveData;

			USerializationSystemLibrary::ConvertObjectToSaveData(Pair.Value, ObjectSaveData);

			PresetSaveData.InputObjectsSaveData.Add(Pair.Key->Tag, ObjectSaveData);
		}

		SaveData.Add(PresetPair.Key->Tag, PresetSaveData);
	}

	return true;
}

bool UTouchInputsStat::PostConvertFromSaveData_Implementation()
{
	const UManagersSystem* ManagersSystem = GetManagersSystem();
	if(!ManagersSystem) return false;

	const UDataManager* DataManager = ManagersSystem->GetManager<UDataManager>();
	if(!DataManager) return false;
	
	InputPresets.Empty();

	for(const TTuple<FName, FTouchInputPresetSaveData>& PresetPair : SaveData)
	{
		UTouchInputPresetSlot* PresetSlot = DataManager->GetDataAsset<UTouchInputPresetSlot>(PresetPair.Key);
		if(!PresetSlot) continue;
		
		FTouchInputPreset Preset;

		for(const TTuple<FName, FObjectSaveData>& Pair : PresetPair.Value.InputObjectsSaveData)
		{
			UTouchInputSlotData* Slot = DataManager->GetDataAsset<UTouchInputSlotData>(Pair.Key);
			if(!Slot) continue;

			UTouchInputSaveObject* SaveObject = USerializationSystemLibrary::ConvertSaveDataToObject<UTouchInputSaveObject>(Pair.Value, this);
			if(!SaveObject) continue;

			Preset.InputObjects.Add(Slot, SaveObject);
		}

		InputPresets.Add(PresetSlot, Preset);
	}
	
	return true;
}

bool UTouchInputsStat::HasPreset(UTouchInputPresetSlot* Preset) const
{
	return InputPresets.Contains(Preset);
}

FTouchInputPreset UTouchInputsStat::GetPreset(UTouchInputPresetSlot* Preset) const
{
	const FTouchInputPreset* Out = InputPresets.Find(Preset);

	return Out ? *Out : FTouchInputPreset();
}

TMap<UTouchInputSlotData*, UTouchInputSaveObject*> UTouchInputsStat::GetSlots(UTouchInputPresetSlot* Preset) const
{
	const FTouchInputPreset* Out = InputPresets.Find(Preset);

	return Out ? Out->InputObjects : TMap<UTouchInputSlotData*, UTouchInputSaveObject*>();
}

bool UTouchInputsStat::HasSlotSave(UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot) const
{
	return GetPreset(Preset).InputObjects.Contains(Slot);
}

UTouchInputSaveObject* UTouchInputsStat::GetSlotSave(UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot, TSubclassOf<UTouchInputSaveObject> Class) const
{
	UTouchInputSaveObject* const* SaveObject = GetPreset(Preset).InputObjects.Find(Slot);

	return SaveObject ? *SaveObject : nullptr;
}

void UTouchInputsStat::SaveToSlot(UTouchInputSaveObject* InputObject, UTouchInputPresetSlot* Preset, UTouchInputSlotData* Slot)
{
	if(!InputObject) return;

	InputObject->Rename(nullptr, this);

	if(FTouchInputPreset* PresetPtr = InputPresets.Find(Preset))
	{
		PresetPtr->InputObjects.Add(Slot, InputObject);

		return;
	}

	FTouchInputPreset InputPreset;

	InputPreset.InputObjects.Add(Slot, InputObject);
	
	InputPresets.Add(Preset, InputPreset);
}
