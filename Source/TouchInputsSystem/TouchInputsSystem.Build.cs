// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

using UnrealBuildTool;

public class TouchInputsSystem : ModuleRules
{
	public TouchInputsSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(
			new string[] 
			{
				"TouchInputsSystem/Public/"
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Slate",
				"SlateCore"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"UMG",
				"DataSystem",
				"LogSystem",
				"SaveLoadSystem",
				"SerializationSystem",
				"ManagersSystem"
			}
		);
		
		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
				"DataSystem",
				"LogSystem",
				"SaveLoadSystem",
				"SerializationSystem",
				"ManagersSystem"
			}
		);
	}
}
