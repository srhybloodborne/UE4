// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CActionComponent.h"
#include "CUserWidget_WeaponWidgetKatana.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCUserWidget_WeaponWidgetKatana : public UUserWidget
{
	GENERATED_BODY()

public:
	void ChangeWeapon(EActionType InPrevType, EActionType InNewType);

private:
	//»ö±òº¯°æ
	void Selected();
	void UnSelected();
};
