// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "TouchInputsConfigurationObjects/FreeSwipeTouchInputObject.h"

#include "LogSystem.h"
#include "Module/TouchInputsSystemModule.h"
#include "Components/Inputs/FreeSwipeTouchInputComponent.h"

void UFreeSwipeTouchInputObject::SetTouchInputComponentEssentials()
{
	Super::SetTouchInputComponentEssentials();

	FreeSwipeTouchInputComponent = Cast<UFreeSwipeTouchInputComponent>(TouchInputComponent);

	//FreeSwipeTouchInputComponent->Speed = Speed;
	//FreeSwipeTouchInputComponent->bEnableInterpolation = bEnableInterpolation;
	//FreeSwipeTouchInputComponent->InterpolationSpeed = InterpolationSpeed;

	LOG(LogTouchInputsSystem, "Free Swipe Touch Input Component Essentials Set")
}
