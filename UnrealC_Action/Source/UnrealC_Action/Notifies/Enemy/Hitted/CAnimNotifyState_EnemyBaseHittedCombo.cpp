#include "CAnimNotifyState_EnemyBaseHittedCombo.h"
#include "Global.h"
#include "Characters/CEnemy.h"

FString UCAnimNotifyState_EnemyBaseHittedCombo::GetNotifyName_Implementation() const
{
	return "Next Hitted";
}

void UCAnimNotifyState_EnemyBaseHittedCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->EnableNextHitted();
}

void UCAnimNotifyState_EnemyBaseHittedCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->DisableNextHitted();
}
