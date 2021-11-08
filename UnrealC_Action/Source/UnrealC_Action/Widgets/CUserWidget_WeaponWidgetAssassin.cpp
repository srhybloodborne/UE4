#include "CUserWidget_WeaponWidgetAssassin.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Components/Border.h"

void UCUserWidget_WeaponWidgetAssassin::ChangeWeapon(EActionType InPrevType, EActionType InNewType)
{
	if (InNewType == EActionType::Assassin)
		Selected();
	else if (InPrevType == EActionType::Assassin)
		UnSelected();
}

void UCUserWidget_WeaponWidgetAssassin::Selected()
{
	UBorder* border = Cast<UBorder>(GetWidgetFromName("BG_Border"));
	if (!!border)
	{
		border->SetBrushColor(FLinearColor::Red);
	}
}

void UCUserWidget_WeaponWidgetAssassin::UnSelected()
{
	UBorder* border = Cast<UBorder>(GetWidgetFromName("BG_Border"));
	if (!!border)
	{
		border->SetBrushColor(FLinearColor::White);
	}
}
