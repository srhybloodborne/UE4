#include "CUserWidget_WeaponWidgetBow.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Components/Border.h"

void UCUserWidget_WeaponWidgetBow::ChangeWeapon(EActionType InPrevType, EActionType InNewType)
{
	if (InNewType == EActionType::Bow)
		Selected();
	else if (InPrevType == EActionType::Bow)
		UnSelected();
}

void UCUserWidget_WeaponWidgetBow::Selected()
{
	UBorder* border = Cast<UBorder>(GetWidgetFromName("BG_Border"));
	if (!!border)
	{
		border->SetBrushColor(FLinearColor::Red);
	}
}

void UCUserWidget_WeaponWidgetBow::UnSelected()
{
	UBorder* border = Cast<UBorder>(GetWidgetFromName("BG_Border"));
	if (!!border)
	{
		border->SetBrushColor(FLinearColor::White);
	}
}
