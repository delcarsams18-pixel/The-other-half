using UnrealBuildTool;
public class TheOtherHalfEditorTarget : TargetRules
{
  public TheOtherHalfEditorTarget(TargetInfo Target) : base(Target)
  {
    Type = TargetType.Editor;
    DefaultBuildSettings = BuildSettingsVersion.V5;
    IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
    ExtraModuleNames.Add("TheOtherHalf");
  }
}