#include "CAnimNotify_EnemyBaseHittedEnd.h"
#include "Global.h"
#include "Characters/CEnemy.h"

FString UCAnimNotify_EnemyBaseHittedEnd::GetNotifyName_Implementation() const
{
	return "End Hitted";
}

void UCAnimNotify_EnemyBaseHittedEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->End_BaseHitted();
}