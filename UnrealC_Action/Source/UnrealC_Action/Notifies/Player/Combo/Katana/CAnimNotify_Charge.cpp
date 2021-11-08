#include "CAnimNotify_Charge.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Charge::GetNotifyName_Implementation() const
{
	return "Charge";
}

void UCAnimNotify_Charge::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

//	player->End_Charge();
}