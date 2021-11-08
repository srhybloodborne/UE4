#include "CDoAction_Throw.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CAim.h"
#include "CThrow.h"
#include "Characters/CEnemy.h"

ACDoAction_Throw::ACDoAction_Throw()
{

}

void ACDoAction_Throw::BeginPlay()
{
	Super::BeginPlay();
	//Aim = NewObject<UCAim>();
	//Aim->BeginPlay(OwnerCharacter);
}

void ACDoAction_Throw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Aim->Tick(DeltaTime);
}

void ACDoAction_Throw::DoAction()
{
	//if (Aim->IsAvaliable())
	//	CheckFalse(Aim->InZoom());

	CheckFalse(State->IsIdleMode());

	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	location += FVector(10);

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);

}

void ACDoAction_Throw::End_DoAction()
{
	State->SetIdleMode();
}

void ACDoAction_Throw::OnAim()
{
	//Aim->On();
}

void ACDoAction_Throw::OffAim()
{
	//Aim->Off();
}

void ACDoAction_Throw::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}
