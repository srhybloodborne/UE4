#include "CAnimNotify_DestoryAttachment.h"
#include "Global.h"
#include "Characters/CEnemy.h"

FString UCAnimNotify_DestoryAttachment::GetNotifyName_Implementation() const
{
	return "Destory_Attachment";
}

void UCAnimNotify_DestoryAttachment::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->Destroy_Attachment();
}