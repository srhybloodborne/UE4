#include "CAnimNotifyState_AssassinAirCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Assassin.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_AssassinAirCombo::GetNotifyName_Implementation() const
{
	return "Assassin Air Combo";
}

void UCAnimNotifyState_AssassinAirCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Assassin* assassin = Cast<ACDoAction_Assassin>(action->GetCurrent()->GetAirCombo());

	CheckNull(assassin);

	assassin->EnableCombo();
}

void UCAnimNotifyState_AssassinAirCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Assassin* assassin = Cast<ACDoAction_Assassin>(action->GetCurrent()->GetAirCombo());

	CheckNull(assassin);

	assassin->DisableCombo();
}
