// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Module/TouchInputsSystemModule.h"

#include "LogSystem.h"
#include "Module/TouchInputsSystemSettings.h"

#if UE_EDITOR
#include "ISettingsModule.h"
#endif

IMPLEMENT_MODULE(FTouchInputsSystemModule, TouchInputsSystem)

TOUCHINPUTSSYSTEM_API DEFINE_LOG_CATEGORY(LogTouchInputsSystem);

void FTouchInputsSystemModule::StartupModule()
{
#if UE_EDITOR
	RegisterSystemSettings();
#endif
}

void FTouchInputsSystemModule::ShutdownModule()
{
#if UE_EDITOR
	UnregisterSystemSettings();
#endif
}

#if UE_EDITOR
void FTouchInputsSystemModule::RegisterSystemSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"Touch Inputs System",
			FText::FromString(TEXT("Touch Inputs System")),
			FText::FromString(TEXT("Configuration settings for Touch Inputs System")),
			GetMutableDefault<UTouchInputsSystemSettings>()
		);

		LOG_STATIC(LogTouchInputsSystem, "Touch Inputs System Settings Registered")
	}
}

void FTouchInputsSystemModule::UnregisterSystemSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Touch Inputs System");

		LOG_STATIC(LogTouchInputsSystem, "Touch Inputs System Settings Unregistered")
	}
}
#endif
