// Fill out your copyright notice in the Description page of Project Settings.

using System;
using UnrealBuildTool;
using System.IO;

public class Empyrean : ModuleRules
{
	public Empyrean (ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "Niagara", "OnlineSubSystem"});

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        //Uncomment if you are using online features

         PrivateDependencyModuleNames.Add("OnlineSubsystem");

         string[] PublicPaths = Directory.GetDirectories(ModuleDirectory, "Public", SearchOption.AllDirectories);

         PublicIncludePaths.AddRange(PublicPaths);
    }
}
