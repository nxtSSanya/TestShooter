// TestShooter. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TestShooterEditorTarget : TargetRules
{
	public TestShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TestShooter" } );
	}
}
