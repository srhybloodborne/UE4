#include "CAnimNotify_BeginSecondCombo.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
FString UCAnimNotify_BeginSecondCombo::GetNotifyName_Implementation() const
{
	return "Begin SecondCombo";
}

void UCAnimNotify_BeginSecondCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	action->GetCurrent()->GetSecondCombo()->Begin_DoAction();
}