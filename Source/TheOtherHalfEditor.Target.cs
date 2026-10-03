using UnrealBuildTool;
public class TheOtherHalfEditorTarget : TargetRules
{
	public TheOtherHalfEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("TheOtherHalf");
	}
}