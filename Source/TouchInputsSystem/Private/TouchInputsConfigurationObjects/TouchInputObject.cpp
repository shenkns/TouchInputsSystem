// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "TouchInputsConfigurationObjects/TouchInputObject.h"

#include "LogSystem.h"
#include "Components/Inputs/TouchInputComponent.h"
#include "Data/TouchInputSlotData.h"
#include "Data/TouchInputTypeData.h"
#include "Module/TouchInputsSystemModule.h"

UTouchInputComponent* UTouchInputObject::AddTouchInputComponent(AActor* Owner, UTouchInputSlotData* InputSlot)
{
	Slot = InputSlot;
	
	if(!Slot) return nullptr;
	if(!Slot->TouchInputType) return nullptr;
	
	//const UClass* ComponentClass = Slot->TouchInputType->TouchInputComponentClass.LoadSynchronous();
	//if(!ComponentClass) return nullptr;
	//= NewObject<UTouchInputComponent>(Owner, ComponentClass);

	SetTouchInputComponentEssentials();
	InitTouchInputComponent();
	
	LOG(LogTouchInputsSystem, "%s Initialized With Touch Input Object", *TouchInputComponent->GetName())

	return TouchInputComponent;
}

void UTouchInputObject::SetTouchInputComponentEssentials()
{
	//TouchInputComponent->Name = Slot->Tag;
	//TouchInputComponent->bEnableDebugDrawing = bEnableDebugDrawings;
	//TouchInputComponent->Priority = Priority;

	LOG(LogTouchInputsSystem, "Default Touch Input Component Essentials Set")
}

void UTouchInputObject::InitTouchInputComponent()
{
	TouchInputComponent->RegisterComponent();
	
	//TouchInputComponent->SetupBounds(true, BoundsOrigin, BoundsSize, bBoundsPercentage, true);
}
