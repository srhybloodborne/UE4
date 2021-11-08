#include "CAnimNotifyState_AssassinSecondCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Assassin.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_AssassinSecondCombo::GetNotifyName_Implementation() const
{
	return "Assassin Second Combo";
}

void UCAnimNotifyState_AssassinSecondCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Assassin* assassin = Cast<ACDoAction_Assassin>(action->GetCurrent()->GetSecondCombo());

	CheckNull(assassin);

	assassin->EnableCombo();
}

void UCAnimNotifyState_AssassinSecondCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Assassin* assassin = Cast<ACDoAction_Assassin>(action->GetCurrent()->GetSecondCombo());

	CheckNull(assassin);

	assassin->DisableCombo();
}
