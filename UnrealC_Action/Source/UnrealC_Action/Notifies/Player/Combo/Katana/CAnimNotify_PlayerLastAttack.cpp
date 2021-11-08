#include "CAnimNotify_PlayerLastAttack.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

FString UCAnimNotify_PlayerLastAttack::GetNotifyName_Implementation() const
{
	return "Last Attack";
}

void UCAnimNotify_PlayerLastAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetAirCombo()->LastAttack_DoAtction(true);
	action->GetCurrent()->GetBaseCombo()->LastAttack_DoAtction(true);
	action->GetCurrent()->GetSecondCombo()->LastAttack_DoAtction(true);
}