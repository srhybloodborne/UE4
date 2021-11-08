#include "CAnimNotifyState_BowBaseCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Bow.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_BowBaseCombo::GetNotifyName_Implementation() const
{
	return "Bow Base Combo";
}

void UCAnimNotifyState_BowBaseCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetBaseCombo());

	CheckNull(bow);

	bow->EnableCombo();
}

void UCAnimNotifyState_BowBaseCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetBaseCombo());

	CheckNull(bow);

	bow->DisableCombo();
}
