#include "CAnimNotifyState_AirCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Melee.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_AirCombo::GetNotifyName_Implementation() const
{
	return "AirCombo";
}

void UCAnimNotifyState_AirCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Melee* melee = Cast<ACDoAction_Melee>(action->GetCurrent()->GetAirCombo());

	CheckNull(melee);

	melee->EnableCombo();
}

void UCAnimNotifyState_AirCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Melee* melee = Cast<ACDoAction_Melee>(action->GetCurrent()->GetAirCombo());

	CheckNull(melee);

	melee->DisableCombo();
}
