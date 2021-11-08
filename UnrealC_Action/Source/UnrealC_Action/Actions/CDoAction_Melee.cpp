#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

#include "Characters/CEnemy.h"
#include "Characters/CPlayer.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

void ACDoAction_Melee::DoAction()
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


void ACDoAction_Melee::DoAction(int InIndex)
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

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	bExist = false;


	OwnerCharacter->StopAnimMontage();

	Index++;

	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
}

void ACDoAction_Melee::End_DoAction()
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

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);
	CheckNull(InAttacker);
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


	//CLog::Log(InAttackCauser->GetName());

	//HitStop
	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);//1000ºÐÀÇ 1ÃÊ
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

	FDamageEvent e;

	//If Charge Hit
	//CLog::Print(bChargeSkillHit);
	ACEnemy* enemy = Cast<ACEnemy>(InOtherCharacter);
	if (!!enemy)
	{
		enemy->CheckChargetSkillHitted(bChargeSkillHit);
		enemy->CheckAirAttackHitted(bAirAction);
		enemy->SetLastAttackHitted(bLastAttackAction);

		enemy->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackCauser);

		return;
	}
	
	//Enemy Take Damage

	ACPlayer* player = Cast<ACPlayer>(InOtherCharacter);
	if (!!player)
	{
		player->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackCauser);
		return;
	}
	
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	HittedCharacter.Empty();

}

void ACDoAction_Melee::RestoreGlobalDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void ACDoAction_Melee::Press_Action()
{
	Super::Press_Action();
	CheckFalse(State->IsIdleMode());
	State->SetChargeMode();
	
	Index = 0;

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();

	Index++;
}

void ACDoAction_Melee::Release_Action()
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

void ACDoAction_Melee::Air_DoAction(bool InAirAction)
{
	bAirAction = InAirAction;
}

void ACDoAction_Melee::LastAttack_DoAtction(bool InLastAttackAction)
{
	bLastAttackAction = InLastAttackAction;
}