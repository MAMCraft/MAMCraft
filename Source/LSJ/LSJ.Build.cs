using UnrealBuildTool;

public class LSJ : ModuleRules
{
    public LSJ(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine","NavigationSystem" });
        PrivateDependencyModuleNames.AddRange(new string[] { });

    }
}