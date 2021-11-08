#include "CAnimNotify_EnemyExecution.h"
#include "Global.h"
#include "Characters/CEnemy.h"

FString UCAnimNotify_EnemyExecution::GetNotifyName_Implementation() const
{
	return "End Enemys Exectuion";
}

void UCAnimNotify_EnemyExecution::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->End_Execution();
}