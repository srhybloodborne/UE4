#include "CAnimNotify_EndSecondComboCombo.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"

FString UCAnimNotify_EndSecondComboCombo::GetNotifyName_Implementation() const
{
	return "End SecondCombo";
}

void UCAnimNotify_EndSecondComboCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	action->GetCurrent()->GetSecondCombo()->End_DoAction();
}