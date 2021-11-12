// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EmpyreanEditor : ModuleRules
{
	public EmpyreanEditor (ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] {"EmpyreanEditor/Public"});
		
        PrivateIncludePaths.AddRange(new string[] {"EmpyreanEditor/Private"});

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject",  "Empyrean", "UnrealEd", "UMGEditor"});

        PrivateDependencyModuleNames.AddRange(new string[] { "Empyrean" });

    }
}
