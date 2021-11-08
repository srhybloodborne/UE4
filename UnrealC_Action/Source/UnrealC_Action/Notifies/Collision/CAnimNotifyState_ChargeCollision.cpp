#include "CAnimNotifyState_ChargeCollision.h"
#include "Global.h"
#include "Actions/CAttachment.h"
#include "Actions/CActionData.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_ChargeCollision::GetNotifyName_Implementation() const
{
	return "Charge Collision";
}

void UCAnimNotifyState_ChargeCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetAttachment()->OnChargeCollision();
}

void UCAnimNotifyState_ChargeCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetAttachment()->OffChargeCollision();
}
