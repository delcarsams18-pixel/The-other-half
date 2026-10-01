using UnrealBuildTool;
public class TheOtherHalf : ModuleRules
{
  public TheOtherHalf(ReadOnlyTargetRules Target) : base(Target)
  {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
    PrivateDependencyModuleNames.AddRange(new string[] { "MassAI", "MassCrowd", "PCG", "UMG" });
  }
}using UnrealBuildTool;
public class TheOtherHalfTarget : TargetRules
{
  public TheOtherHalfTarget(TargetInfo Target) : base(Target) { Type = TargetType.Game; DefaultBuildSettings = BuildSettingsVersion.V5; IncludeOrderVersion = EngineIncludeOrderVersion.Latest; ExtraModuleNames.Add("TheOtherHalf"); }
}using UnrealBuildTool;
public class TheOtherHalfEditorTarget : TargetRules
{
  public TheOtherHalfEditorTarget(TargetInfo Target) : base(Target) { Type = TargetType.Editor; DefaultBuildSettings = BuildSettingsVersion.V5; IncludeOrderVersion = EngineIncludeOrderVersion.Latest; ExtraModuleNames.Add("TheOtherHalf"); }
}