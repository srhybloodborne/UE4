#include "CAnimNotifyState_HitStop.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotifyState_HitStop::GetNotifyName_Implementation() const
{
	return "Hit Stop";
}

void UCAnimNotifyState_HitStop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->ExecutionCameraShake();

	UGameplayStatics::SetGlobalTimeDilation(player->GetWorld(), 0.3f);
}

void UCAnimNotifyState_HitStop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	UGameplayStatics::SetGlobalTimeDilation(player->GetWorld(), 1.0f);
}
