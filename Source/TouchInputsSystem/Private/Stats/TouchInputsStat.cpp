// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.


#include "Stats/TouchInputsStat.h"

bool UTouchInputsStat::PreConvertToSaveData_Implementation()
{
	SaveData.Empty();
	
	for(const TTuple<UTouchInputSlotData*, UTouchInputSaveObject*>& Pair : InputsObjects)
	{
		FObjectSaveData ObjectSaveData;
		USerializationSystemLibrary::ConvertObjectToSaveData(Pair.Value, ObjectSaveData);

		SaveData.Add(Pair.Key, ObjectSaveData);
	}

	return true;
}

bool UTouchInputsStat::PostConvertFromSaveData_Implementation()
{
	InputsObjects.Empty();

	for(const TTuple<UTouchInputSlotData*, FObjectSaveData>& Pair : SaveData)
	{
		InputsObjects.Add(Pair.Key, USerializationSystemLibrary::ConvertSaveDataToObject<UTouchInputSaveObject>(Pair.Value, this));
	}
	
	return true;
}

bool UTouchInputsStat::HasSlotSave(UTouchInputSlotData* Slot) const
{
	return InputsObjects.Contains(Slot);
}

UTouchInputSaveObject* UTouchInputsStat::GetSlotSave(UTouchInputSlotData* Slot, TSubclassOf<UTouchInputSaveObject> Class) const
{
	if(!HasSlotSave(Slot)) return nullptr;

	UTouchInputSaveObject* const* SaveObject = InputsObjects.Find(Slot);

	return SaveObject ? *SaveObject : nullptr;
}
