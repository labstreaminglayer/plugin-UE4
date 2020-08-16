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
        get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "..", "ThirdParty")); }
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
        PrivateIncludePaths.AddRange(new string[]
        {
            "LSL/Private",
            Path.Combine(ThirdPartyPath, "liblsl", "include"),
            // ... add other private include paths required here ...
        });

        PublicIncludePaths.AddRange(new string[]
        {
            "LSL/Public",
            "LSL/Classes",
            Path.Combine(ThirdPartyPath, "liblsl", "include"),
            // ... add public include paths required here ...
        });

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Engine",
            "Core",
            "CoreUObject",
            "InputCore",
            "Projects",
            // ... add other public dependencies that you statically link with here ...
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            // ... add private dependencies that you statically link with here ...
            // "LabStreamingLayer",
        });

        DynamicallyLoadedModuleNames.AddRange(new string[]
        {
            // ... add any modules that your module loads dynamically here ...
        });

        LoadLSLLib(Target);
    }

    public bool LoadLSLLib(ReadOnlyTargetRules Target)
    {
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "lsl.lib"));
            PublicDelayLoadDLLs.Add("lsl.dll");
            RuntimeDependencies.Add(Path.Combine(DllPath, "bin", "lsl.dll"));
		}

        return true;

        //TODO: Copy dll/dylib/so to Project_name\Binaries\Platform\
    }
}

}
