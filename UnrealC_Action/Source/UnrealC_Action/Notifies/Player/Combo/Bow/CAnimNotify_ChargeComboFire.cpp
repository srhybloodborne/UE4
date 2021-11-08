#include "CAnimNotify_ChargeComboFire.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Bow.h"

FString UCAnimNotify_ChargeComboFire::GetNotifyName_Implementation() const
{
	return "Charge Combo Fire";
}

void UCAnimNotify_ChargeComboFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetChargeCombo());

	CheckNull(bow);

	bow->Fire();
}