#include "CAnimNotify_EnemyExhausted.h"
#include "Global.h"
#include "Characters/CEnemy.h"

FString UCAnimNotify_EnemyExhausted::GetNotifyName_Implementation() const
{
	return "End Enemys Exhausted";
}

void UCAnimNotify_EnemyExhausted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->End_Exhausted();
}