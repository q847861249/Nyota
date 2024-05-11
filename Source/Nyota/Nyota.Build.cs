// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Nyota : ModuleRules
{
	public Nyota(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput", 
            "GameplayAbilities",
            "EnhancedInput",
            "AIModule",
            "GameplayTasks",
            "GameplayTags",
            "InputCore",
            "AnimGraphRuntime",
            "MotionWarping",});
	}
}
