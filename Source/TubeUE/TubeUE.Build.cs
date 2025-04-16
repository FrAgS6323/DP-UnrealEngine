// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TubeUE : ModuleRules
{
	public TubeUE(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "EnhancedInput", "HTTP", "Json", "Slate", "SlateCore" });

        PublicAdditionalLibraries.Add("dxgi.lib");
        PublicAdditionalLibraries.Add("d3d12.lib");

        PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "RenderCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
