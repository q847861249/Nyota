// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NyotaEditorTarget : TargetRules
{
	public NyotaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("Nyota");
	}
}
