#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin Action";
}	

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	if(!!action->GetCurrent() && !!action->GetCurrent()->GetBaseCombo())
		action->GetCurrent()->GetBaseCombo()->Begin_DoAction();
}