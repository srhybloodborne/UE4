#include "CAnimNotifyState_BowSecondCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Melee.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_BowSecondCombo::GetNotifyName_Implementation() const
{
	return "Bow Second Combo";
}

void UCAnimNotifyState_BowSecondCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetSecondCombo());

	CheckNull(bow);

	bow->EnableCombo();
}

void UCAnimNotifyState_BowSecondCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetSecondCombo());

	CheckNull(bow);

	bow->DisableCombo();
}
	