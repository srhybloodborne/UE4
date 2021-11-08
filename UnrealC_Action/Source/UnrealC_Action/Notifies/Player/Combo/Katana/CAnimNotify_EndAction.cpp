#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End Action";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	if (!!action->GetCurrent() && !!action->GetCurrent()->GetBaseCombo())
		action->GetCurrent()->GetBaseCombo()->End_DoAction();
}