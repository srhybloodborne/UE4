// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CActionComponent.h"
#include "CUserWidget_WeaponWidget.generated.h"



USTRUCT(BlueprintType)
struct FUserWidget_WeaponWidgetData
{
	GENERATED_BODY()
public:
	class UCUserWidget_WeaponWidgetKatana* katana;
	class UCUserWidget_WeaponWidgetAssassin* assassin;
	class UCUserWidget_WeaponWidgetBow* bow;
};
/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCUserWidget_WeaponWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	void NativeConstruct() override;

public:
	UFUNCTION()
		void ChangeWeapon(EActionType InPrevType, EActionType InNewType);

private:
	FUserWidget_WeaponWidgetData Data;
};
