// Copyright (c) 2021 Chadwick Boulay

using UnrealBuildTool;
using System.IO;

namespace UnrealBuildTool.Rules
{

public class LSL : ModuleRules
{
    private string ModulePath
    {
         get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "ThirdParty")); }
    }

    private string BinariesPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "..", "Binaries")); }
    }

    private string LibraryPath
    {
        get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "liblsl", "lib")); }
    }

    private string DllPath
    {
        get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "liblsl", "bin")); }
    }

    public LSL(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        
        PrivateIncludePaths.AddRange(new string[]
        {
            Path.Combine(ModuleDirectory, "Private"),
            Path.Combine(ThirdPartyPath, "liblsl", "include"),
        });

        PublicIncludePaths.AddRange(new string[]
        {
            Path.Combine(ModuleDirectory, "Public"),
            Path.Combine(ThirdPartyPath, "liblsl", "include"),
        });

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Engine",
            "Core",
            "CoreUObject",
            "InputCore",
            "Projects",
        });

        LoadLSLLib(Target);
    }

    public bool LoadLSLLib(ReadOnlyTargetRules Target)
    {
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "lsl.lib"));
            PublicDelayLoadDLLs.Add("lsl.dll");
            RuntimeDependencies.Add(Path.Combine(DllPath, "lsl.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(DllPath, "liblsl.dylib"));
            RuntimeDependencies.Add(Path.Combine(DllPath, "liblsl.dylib"));
        }

        return true;
    }
}

}
