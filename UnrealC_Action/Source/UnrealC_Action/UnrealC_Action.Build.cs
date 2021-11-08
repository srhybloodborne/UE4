// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealC_Action : ModuleRules
{
	public UnrealC_Action(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", "CoreUObject", "Engine", "InputCore" , "Niagara", "AIModule", "GameplayTasks",
		});

		//Build �� �ȸ����� visual studio sin ���� ������ generate �ٽ�
		
		PublicIncludePaths.Add(ModuleDirectory);
	}
}
