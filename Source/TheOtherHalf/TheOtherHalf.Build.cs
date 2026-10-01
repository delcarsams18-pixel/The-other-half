using UnrealBuildTool;
using System.Collections.Generic;

public class TheOtherHalfTarget : TargetRules
{
	public TheOtherHalfTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("TheOtherHalf");
	}
}