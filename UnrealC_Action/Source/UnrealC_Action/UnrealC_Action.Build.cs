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

		//Build 가 안먹을땐 visual studio sin 파일 삭제후 generate 다시
		
		PublicIncludePaths.Add(ModuleDirectory);
	}
}
