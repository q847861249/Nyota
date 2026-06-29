// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Nyota : ModuleRules
{
	public Nyota(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NetCore", "Niagara", "UMG", "AIModule",
			"NavigationSystem", "ModularGameplay", "GameFeatures", "UnLua", "Puerts", "JsEnv", "ModularGameplayActors", "GameplayAbilities"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"GameplayAbilities", "GameplayTags", "AnimGraphRuntime", "GameplayTasks", "CommonUI", "CommonInput"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}