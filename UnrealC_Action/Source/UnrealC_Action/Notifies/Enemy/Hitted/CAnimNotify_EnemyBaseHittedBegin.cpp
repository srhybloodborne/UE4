#include "CAnimNotify_EnemyBaseHittedBegin.h"
#include "Global.h"
#include "Characters/CEnemy.h"

FString UCAnimNotify_EnemyBaseHittedBegin::GetNotifyName_Implementation() const
{
	return "Begin Hitted";
}

void UCAnimNotify_EnemyBaseHittedBegin::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->Begin_BaseHitted();
}