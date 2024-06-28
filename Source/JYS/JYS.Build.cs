using UnrealBuildTool;

public class JYS : ModuleRules
{
    public JYS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "NavigationSystem", "AIModule", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { });

    }
}