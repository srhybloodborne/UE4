#include "CAnimNotify_BeginAirCombo.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
FString UCAnimNotify_BeginAirCombo::GetNotifyName_Implementation() const
{
	return "Begin AirCombo";
}

void UCAnimNotify_BeginAirCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);
	
	action->GetCurrent()->GetAirCombo()->Begin_DoAction();
}