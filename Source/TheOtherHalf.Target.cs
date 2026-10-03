using UnrealBuildTool;
public class TheOtherHalfTarget : TargetRules
{
	public TheOtherHalfTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("TheOtherHalf");
	}
}