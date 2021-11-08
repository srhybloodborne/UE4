#include "CUserWidget_WeaponWidgetKatana.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Components/Border.h"

void UCUserWidget_WeaponWidgetKatana::ChangeWeapon(EActionType InPrevType, EActionType InNewType)
{
	if (InNewType == EActionType::Katana)
		Selected();
	else if (InPrevType == EActionType::Katana)
		UnSelected();
}

void UCUserWidget_WeaponWidgetKatana::Selected()
{
	UBorder* border = Cast<UBorder>(GetWidgetFromName("BG_Border"));
	if (!!border)
	{
		border->SetBrushColor(FLinearColor::Red);
	}
}

void UCUserWidget_WeaponWidgetKatana::UnSelected()
{
	UBorder* border = Cast<UBorder>(GetWidgetFromName("BG_Border"));
	if (!!border)
	{
		border->SetBrushColor(FLinearColor::White);
	}
}
