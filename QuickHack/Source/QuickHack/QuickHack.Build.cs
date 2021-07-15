// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuickHack : ModuleRules
{
	public QuickHack(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] {"GameplayAbilities", "GameplayTags", "GameplayTasks", "AIModule"});
	}
}
