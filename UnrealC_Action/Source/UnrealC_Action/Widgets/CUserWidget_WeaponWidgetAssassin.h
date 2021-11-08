// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CActionComponent.h"
#include "CUserWidget_WeaponWidgetAssassin.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCUserWidget_WeaponWidgetAssassin : public UUserWidget
{
	GENERATED_BODY()

public:
	void ChangeWeapon(EActionType InPrevType, EActionType InNewType);

private:
	//���򺯰�
	void Selected();
	void UnSelected();
};
