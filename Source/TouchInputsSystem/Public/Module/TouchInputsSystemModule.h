// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#pragma once

#include "Modules/ModuleManager.h"

TOUCHINPUTSSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogTouchInputsSystem, Log, All);

class FTouchInputsSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

#if UE_EDITOR
	// Init System Configuration
	void RegisterSystemSettings() const;
	void UnregisterSystemSettings() const;
#endif
};
