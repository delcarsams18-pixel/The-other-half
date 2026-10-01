using UnrealBuildTool;
public class TheOtherHalf : ModuleRules
{
  public TheOtherHalf(ReadOnlyTargetRules Target) : base(Target)
  {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
    PrivateDependencyModuleNames.AddRange(new string[] { "MassAI", "MassCrowd", "PCG", "UMG" });
  }
}