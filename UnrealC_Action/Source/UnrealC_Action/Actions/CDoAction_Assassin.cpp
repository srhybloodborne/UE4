#include "CDoAction_Assassin.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "CThrow.h"
#include "Camera/CameraComponent.h"

void ACDoAction_Assassin::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);
	if (bEnable == true)
	{
		bExist = true;
		bEnable = false;

		return;
	}

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	Index = 0;
	bLastAttackAction = false;

	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
}

void ACDoAction_Assassin::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	bExist = false;


	OwnerCharacter->StopAnimMontage();

	Index++;

	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
}

void ACDoAction_Assassin::End_DoAction()
{
	Super::End_DoAction();
	OwnerCharacter->StopAnimMontage();

	Index = -1;
	bChargeSkillHit = false;
	bAirAction = false;
	bLastAttackAction = false;
	State->SetIdleMode();
	Status->SetMove();

}

void ACDoAction_Assassin::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	for (const ACharacter* other : HittedCharacter)
	{
		if (InOtherCharacter == other)
		{

			return;
		}
	}
	HittedCharacter.Add(InOtherCharacter);

	if (Index < 0)
		return;


	CLog::Log(InAttackCauser->GetName());

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
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	//Niagara Effect

	UNiagaraSystem* hitEffect_niagara = Datas[Index].Effect_Niagara;
	if (!!hitEffect_niagara)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitEffect_niagara, transform.GetLocation(), transform.GetRotation().Rotator());
	}


	//CameraShake
	TSubclassOf<UCameraShake> shake = Datas[Index].ShakeClass;
	if (!!shake)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);
	}


	//If Charge Hit
	//CLog::Print(bChargeSkillHit);
	Cast<ACEnemy>(InOtherCharacter)->CheckChargetSkillHitted(bChargeSkillHit);
	Cast<ACEnemy>(InOtherCharacter)->CheckAirAttackHitted(bAirAction);
	Cast<ACEnemy>(InOtherCharacter)->SetLastAttackHitted(bLastAttackAction);

	//Enemy Take Damage
	FDamageEvent e;
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackCauser);

}

void ACDoAction_Assassin::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	HittedCharacter.Empty();

}

void ACDoAction_Assassin::RestoreGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void ACDoAction_Assassin::Press_Action()
{
	Super::Press_Action();
	CheckFalse(State->IsIdleMode());
	State->SetChargeMode();

	Index = 0;

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();

	Index++;
}

void ACDoAction_Assassin::Release_Action()
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
		HittedCharacter.Empty();
	}
}

void ACDoAction_Assassin::Air_DoAction(bool InAirAction)
{
	bAirAction = InAirAction;
}

void ACDoAction_Assassin::LastAttack_DoAtction(bool InLastAttackAction)
{
	bLastAttackAction = InLastAttackAction;
}



void ACDoAction_Assassin::Fire()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Fire
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow");

	//TODO: 화살방향 조절해야됨, 플레이어의 방향과 Aim 카메라의 크로스헤드와 괴리있음
	//UCameraComponent* Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	//FRotator temp_A = OwnerCharacter->GetController()->GetControlRotation();
	//FRotator temp_B = Camera->GetComponentRotation();


	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	rotator = rotator.Add(0, 5, 0);//대충 기워붙여서 조정햇음
	//location += FVector(100);
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));


	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;


	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[Index].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActor<ACThrow>(Datas[0].ThrowClass, transform, params);
	throwObject->SetChargeSkill(false);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Assassin::OnThrowBeginOverlap);
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

void ACDoAction_Assassin::OnThrowBeginOverlap(FHitResult InHitResult)
{
	/*FDamageEvent e;
	Cast<ACEnemy>(InHitResult.GetActor())->SetLastAttackHitted(false);
	Cast<ACEnemy>(InHitResult.GetActor())->CheckChargetSkillHitted(false);

	InHitResult.GetActor()->TakeDamage(Datas[Index].Power, e, OwnerCharacter->GetController(), this);
	if (bLastAttackAction)
		bLastAttackAction = false;*/

	FDamageEvent e;

	//ACPlayer* player = Cast<ACPlayer>(InHitResult.GetActor());
	//if (!!player)
	//	return;

	ACEnemy* enemy = Cast<ACEnemy>(InHitResult.GetActor());

	if (enemy != NULL)
	{
		enemy->SetLastAttackHitted(bLastAttackAction);
		enemy->CheckChargetSkillHitted(bChargeSkillHit);
		//enemy->SetBowHitted(true);

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