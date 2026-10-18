// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

/** @brief 声明游戏模块的编译依赖，使原生玩法代码能够访问所需的引擎功能。 */
public class Nyota : ModuleRules
{
	/** @brief 在 UBT 构建目标时配置预编译头及模块链接依赖。 */
	public Nyota(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NetCore", "Niagara", "UMG", "AIModule",
			"NavigationSystem", "ModularGameplay", "GameFeatures", "ModularGameplayActors", "GameplayAbilities"
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
