#include "CDoAction_Bow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
//#include "CAim.h"
#include "CThrow.h"
#include "Camera/CameraComponent.h"
#include "Characters/CEnemy.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
ACDoAction_Bow::ACDoAction_Bow()
{

}

void ACDoAction_Bow::BeginPlay()
{
	Super::BeginPlay();
	//Aim = NewObject<UCAim>();
	//Aim->BeginPlay(OwnerCharacter);
}

void ACDoAction_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Aim->Tick(DeltaTime);
}

void ACDoAction_Bow::DoAction()
{
	
	//if (Aim->IsAvaliable())
	//{
	//	CheckFalse(Aim->InZoom());
	//}

	Super::DoAction();
	CheckFalse(Datas.Num() > 0);
	if (bEnable == true)
	{
		bExist = true;
		bEnable = false;

		return;
	}

	if (bAirAttack == true && bLacunCharaacter == true)
	{
		FVector start = GetActorLocation();
		FVector target = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 100);

		
		//Impulse Move
		if (t_count >= 2)
			Temp_Count = 700;
		FVector direction = target - start;
		direction.Normalize();
		OwnerCharacter->LaunchCharacter(direction * Temp_Count, true, false);
		Temp_Count -= 150;
		t_count++;
		bAirAttack = false;
		bLacunCharaacter = false;
	}  

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	Index = 0;

	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
}
void ACDoAction_Bow::DoAction(int InIndex)
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);
	if (bEnable == true)
	{
		bExist = true;
		bEnable = false;

		return;
	}
	Index = InIndex;
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();
	bLastAttackAction = false;

	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
}

void ACDoAction_Bow::Begin_DoAction()
{

	CheckFalse(bExist);
	bExist = false;

	if (bAirAttack == true && bLacunCharaacter == true)
	{
		FVector start = GetActorLocation();
		FVector target = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 100);


		//Impulse Move
		FVector direction = target - start;
		direction.Normalize();
		OwnerCharacter->LaunchCharacter(direction * 50, true, false);
	
		bAirAttack = false;
		bLacunCharaacter = false;
	}

	OwnerCharacter->StopAnimMontage();

	Index++;

	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
}

void ACDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();
	Index = -1;
	OwnerCharacter->StopAnimMontage();
	State->SetIdleMode();
	Status->SetMove();
	bLastAttackAction = false;
	bAirAttack = false;
	bChargeSkillHit = false;
	Temp_Count = 700;
	t_count = 0;
	//OwnerCharacter->bUseControllerRotationYaw = true;

}


void ACDoAction_Bow::Fire()
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fire
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow");

	//TODO: 화살방향 조절해야됨, 플레이어의 방향과 Aim 카메라의 크로스헤드와 괴리있음
	//UCameraComponent* Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	//FRotator temp_A = OwnerCharacter->GetController()->GetControlRotation();
	//FRotator temp_B = Camera->GetComponentRotation();

	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	rotator = rotator.Add(0,5,0);//대충 기워붙여서 조정햇음
	//location += FVector(100);
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));


	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActor<ACThrow>(Datas[0].ThrowClass, transform, params);
	throwObject->SetChargeSkill(false);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Bow::OnThrowBeginOverlap);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);

	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	   Player
	//HitStop
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);//1000분의 1초
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalDilation", hitStop * 0.3f, false);
	}

	//Effect
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(this->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	//CameraShake
	TSubclassOf<UCameraShake> shake = Datas[Index].ShakeClass;
	if (!!shake)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);
	}
}

void ACDoAction_Bow::OnAim()
{
	//Aim->On();
}

void ACDoAction_Bow::OffAim()
{
	//Aim->Off();
}

void ACDoAction_Bow::RestoreGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}


void ACDoAction_Bow::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	
	//ACPlayer* player = Cast<ACPlayer>(InHitResult.GetActor());
	//if (!!player)
	//	return;

	ACEnemy* enemy = Cast<ACEnemy>(InHitResult.GetActor());

	if (enemy != NULL)
	{
		enemy->SetLastAttackHitted(bLastAttackAction);
		enemy->CheckChargetSkillHitted(bChargeSkillHit);
		enemy->SetBowHitted(true);

		enemy->TakeDamage(Datas[Index].Power, e, OwnerCharacter->GetController(), this);


		if (bLastAttackAction)
			bLastAttackAction = false;

		return;
	}

	ACPlayer* player = Cast<ACPlayer>(InHitResult.GetActor());
	if (player != NULL)
	{
		player->TakeDamage(Datas[Index].Power, e, OwnerCharacter->GetController(), this);

	}
	


	if (bLastAttackAction)
		bLastAttackAction = false;


}

void ACDoAction_Bow::LastAttack_DoAtction(bool InLastAttackAction)
{
	bLastAttackAction = InLastAttackAction;
}

void ACDoAction_Bow::Air_DoAction(bool InAirAction)
{
	bAirAttack = InAirAction;
}

//TODO: AirCombo맞앗을떄 Enemy Rotator NP


void ACDoAction_Bow::Press_Action()
{
	Super::Press_Action();
	CheckFalse(State->IsIdleMode());
	State->SetChargeMode();

	Index = 0;

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();

	Index++;
}

void ACDoAction_Bow::Release_Action()
{
	Super::Release_Action();
	if (!State->IsChargeMode())
		return;
	OwnerCharacter->StopAnimMontage();
	if (Datas.Num() > 1)
	{
		OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
		Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
		bChargeSkillHit = true;
	}
	else
	{
		State->SetIdleMode();
		Status->SetMove();
		Index = -1;
		//HittedCharacter.Empty();
	}
}