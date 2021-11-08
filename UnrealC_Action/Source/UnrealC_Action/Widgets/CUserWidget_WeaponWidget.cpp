
#include "CUserWidget_WeaponWidget.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "Widgets/CUserWidget_WeaponWidgetKatana.h"
#include "Widgets/CUserWidget_WeaponWidgetAssassin.h"
#include "Widgets/CUserWidget_WeaponWidgetBow.h"
#include "Components/Border.h"

void UCUserWidget_WeaponWidget::NativeConstruct()
{
	UGridPanel* gridPanel = Cast<UGridPanel>(GetWidgetFromName("Grid"));
	TArray<UWidget*> widgets = gridPanel->GetAllChildren();


	for (UWidget* widget : widgets)
	{
		FUserWidget_WeaponWidgetData data;

		UCUserWidget_WeaponWidgetKatana* temp_katana = Cast<UCUserWidget_WeaponWidgetKatana>(widget);
		if(!!temp_katana)
			data.katana = temp_katana;

		UCUserWidget_WeaponWidgetAssassin* temp_assassin = Cast<UCUserWidget_WeaponWidgetAssassin>(widget);
		if (!!temp_assassin)
			data.assassin = temp_assassin;

		UCUserWidget_WeaponWidgetBow* temp_bow = Cast<UCUserWidget_WeaponWidgetBow>(widget);
		if (!!temp_bow)
			data.bow = temp_bow;

		Data = data;
	}

	Super::NativeConstruct();
}

void UCUserWidget_WeaponWidget::ChangeWeapon(EActionType InPrevType, EActionType InNewType)
{
	Data.katana->ChangeWeapon(InPrevType, InNewType);
	Data.assassin->ChangeWeapon(InPrevType, InNewType);
	Data.bow->ChangeWeapon(InPrevType, InNewType);
}
