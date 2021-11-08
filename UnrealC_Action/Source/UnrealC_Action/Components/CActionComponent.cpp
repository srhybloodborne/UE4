#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "GameFramework/Character.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"

UCActionComponent::UCActionComponent()
{
	
}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(character, &Datas[i]);
	}
	
}

void UCActionComponent::SetUnarmedMode()
{
	if (!!Datas[(int32)Type])
		Datas[(int32)Type]->GetEquipment()->UnEquip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{

	SetMode(EActionType::Fist);
}

void UCActionComponent::SetKatanaMode()
{
	SetMode(EActionType::Katana);
}

void UCActionComponent::SetScabarrdMode()
{
	SetMode(EActionType::Scabarrd);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetFireStormMode()
{
	SetMode(EActionType::FireStorm);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetIceballMode()
{
	SetMode(EActionType::Iceball);
}

void UCActionComponent::SetBowMode()
{
	SetMode(EActionType::Bow);
}

void UCActionComponent::SetQuiverMode()
{
	SetMode(EActionType::Quiver);
}

void UCActionComponent::SetAssassinMode()
{
	SetMode(EActionType::Assassin);
}

void UCActionComponent::SetAssassinScabardMode()
{
	SetMode(EActionType::Assassin_Scabard);
}

void UCActionComponent::SetMode(EActionType InNewType)
{
	if (Type == InNewType)//기존무기 장착해제
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)//다른무기 장착
	{
		Datas[(int32)Type]->GetEquipment()->UnEquip();
	}
	
	if (!!Datas[(int32)InNewType] && !!Datas[(int32)InNewType]->GetEquipment())
	{
		Datas[(int32)InNewType]->GetEquipment()->Equip();
	}
	ChangeType(InNewType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);	
}

void UCActionComponent::BaseCombo()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetBaseCombo();

		if (!!action)
			action->DoAction();
	}
}


void UCActionComponent::BaseCombo(int Index)
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetBaseCombo();

		if (!!action)
			action->DoAction(Index);
	}
}
void UCActionComponent::AirCombo()
{
	CheckTrue(IsUnarmedMode());
	
	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetAirCombo();

		if (!!action)
		{
			action->DoAction();
			action->Air_DoAction(true);
		}
	}
}

void UCActionComponent::SecondCombo()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetSecondCombo();
		
		if (!!action)
			action->DoAction();

	}
}

void UCActionComponent::ChargeComboPress()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetChargeCombo();

		if (!!action)
			action->Press_Action();

	}
}

void UCActionComponent::OnAllCollision()
{
	for (UCAction* data : Datas)
	{
		if (!!data == false)
			continue;

		if (!!data->GetAttachment() == false)
			continue;

		data->GetAttachment()->OnCollision();
	}
}

void UCActionComponent::OffAllCollision()
{
	for (UCAction* data : Datas)
	{
		if (!!data == false)
			continue;
		
		if (!!data->GetAttachment() == false)
			continue;

		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::ChargeComboRelease()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetChargeCombo();

		if (!!action)
			action->Release_Action();

	}
}

void UCActionComponent::DoAim(bool InAim)
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetBaseCombo();

		if (!!action)
			InAim ? action->OnAim() : action->OffAim();

		 action = Datas[(int32)Type]->GetSecondCombo();

		if (!!action)
			InAim ? action->OnAim() : action->OffAim();

		 action = Datas[(int32)Type]->GetAirCombo();

		if (!!action)
			InAim ? action->OnAim() : action->OffAim();

	}
}

void UCActionComponent::Damaged()
{

}

void UCActionComponent::Dead()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i] && !!Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::End_Dead()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i] && !!Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();
	}
}

