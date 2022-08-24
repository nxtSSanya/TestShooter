// TestShooter. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TestShooterTarget : TargetRules
{
	public TestShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TestShooter" } );
	}
}
