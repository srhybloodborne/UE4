#include "CAnimNotifyState_BowAirCombo.h"
#include "Global.h"
#include "Actions/CDoAction_Bow.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_BowAirCombo::GetNotifyName_Implementation() const
{
	return "BowAirCombo";
}

void UCAnimNotifyState_BowAirCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetAirCombo());

	CheckNull(bow);

	bow->EnableCombo();
}

void UCAnimNotifyState_BowAirCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetAirCombo());

	CheckNull(bow);

	bow->DisableCombo();
}
