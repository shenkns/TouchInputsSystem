// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "TouchInputsConfigurationObjects/ButtonTouchInputObject.h"

#include "LogSystem.h"
#include "Components/Inputs/ButtonTouchInputComponent.h"
#include "Module/TouchInputsSystemModule.h"

void UButtonTouchInputObject::SetTouchInputComponentEssentials()
{
	Super::SetTouchInputComponentEssentials();

	ButtonTouchInputComponent = Cast<UButtonTouchInputComponent>(TouchInputComponent);

	ButtonTouchInputComponent->DoubleTapInterval = DoubleTapInterval;
	ButtonTouchInputComponent->TwoFingersTapInterval = TwoFingersTapInterval;

	LOG(LogTouchInputsSystem, "Button Touch Input Component Essentials Set")
}

void UButtonTouchInputObject::InitTouchInputComponent()
{
	Super::InitTouchInputComponent();

	ButtonTouchInputComponent->SetupBackgroundsByBounds(ButtonTexture, Tint);
}
