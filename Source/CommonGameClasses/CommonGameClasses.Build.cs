// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonGameClasses : ModuleRules
{
	public CommonGameClasses(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GameplayTags",
				"NavigationSystem",
				"GameplayTasks",
				"AIModule",
				"Niagara",
				"ALS", 
				"ALSCamera",
				"SMSystem", 
				"EnhancedInput"
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"AnimGraphRuntime"
			}
		);
	}
}
