#include "CAnimNotifyState_AssassinBaseCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Assassin.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_AssassinBaseCombo::GetNotifyName_Implementation() const
{
	return "Assassin Base Combo";
}

void UCAnimNotifyState_AssassinBaseCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Assassin* assassin = Cast<ACDoAction_Assassin>(action->GetCurrent()->GetBaseCombo());

	CheckNull(assassin);

	assassin->EnableCombo();
}

void UCAnimNotifyState_AssassinBaseCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Assassin* assassin = Cast<ACDoAction_Assassin>(action->GetCurrent()->GetBaseCombo());

	CheckNull(assassin);

	assassin->DisableCombo();
}
