#include "CAnimNotifyState_SecondCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Melee.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_SecondCombo::GetNotifyName_Implementation() const
{
	return "Second Combo";
}

void UCAnimNotifyState_SecondCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Melee* melee = Cast<ACDoAction_Melee>(action->GetCurrent()->GetSecondCombo());

	CheckNull(melee);

	melee->EnableCombo();
}

void UCAnimNotifyState_SecondCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Melee* melee = Cast<ACDoAction_Melee>(action->GetCurrent()->GetSecondCombo());

	CheckNull(melee);

	melee->DisableCombo();
}
	