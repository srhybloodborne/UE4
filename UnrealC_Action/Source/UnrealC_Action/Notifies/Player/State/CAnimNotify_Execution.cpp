#include "CAnimNotify_Execution.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_Execution::GetNotifyName_Implementation() const
{
	return "End Player Exectuion";
}

void UCAnimNotify_Execution::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Execution();
}