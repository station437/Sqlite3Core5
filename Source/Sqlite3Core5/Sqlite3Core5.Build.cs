// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Sqlite3Core5 : ModuleRules
{
	public Sqlite3Core5(ReadOnlyTargetRules Target) : base(Target)
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
				//"Sqlite3Core5Library",
				"Projects"
				//"DeveloperSettings"
				// ... add other public dependencies that you statically link with here ...
			}
			);			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
				"CoreUObject",
				"Engine"
			}
			);
				
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine("$(PluginDir)/Resources/Extern/lib/win64/sqlite3.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("sqlite3.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Resources/Extern/lib/win64/sqlite3.dll");
		}
		
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string LinuxSoPath = Path.Combine("$(PluginDir)", "Resources", "Extern", "lib", "linux", "sqlite3.so");
			PublicAdditionalLibraries.Add(LinuxSoPath);
			PublicDelayLoadDLLs.Add(LinuxSoPath);
			RuntimeDependencies.Add(LinuxSoPath);
		}

		PublicIncludePaths.Add(Path.Combine("$(PluginDir)", "Source", "Extern"));
	}
}
