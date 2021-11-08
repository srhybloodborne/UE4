#include "CAnimNotify_AirComboLaunch.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CDoAction_Bow.h"

FString UCAnimNotify_AirComboLaunch::GetNotifyName_Implementation() const
{
	return "Launch";
}

void UCAnimNotify_AirComboLaunch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Bow* bow = Cast<ACDoAction_Bow>(action->GetCurrent()->GetAirCombo());

	CheckNull(bow);

	bow->EnableLaunchCharacter();
}