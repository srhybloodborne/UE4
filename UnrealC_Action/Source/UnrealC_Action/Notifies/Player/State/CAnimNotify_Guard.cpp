#include "CAnimNotify_Guard.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Guard::GetNotifyName_Implementation() const
{
	return "Guard";
}

void UCAnimNotify_Guard::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Guard();
}