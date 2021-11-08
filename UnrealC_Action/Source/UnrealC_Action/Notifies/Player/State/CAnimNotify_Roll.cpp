#include "CAnimNotify_Roll.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Roll::GetNotifyName_Implementation() const
{
	return "Roll";
}

void UCAnimNotify_Roll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Roll();
}