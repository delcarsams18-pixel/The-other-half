using UnrealBuildTool;
public class PulsePlaitTheOtherHalf : ModuleRules
{
    public PulsePlaitTheOtherHalf(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule" });
    }
}