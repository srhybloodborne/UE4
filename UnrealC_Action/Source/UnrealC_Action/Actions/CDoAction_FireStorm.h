// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_FireStorm.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API ACDoAction_FireStorm : public ACDoAction
{
	GENERATED_BODY()

public:
	ACDoAction_FireStorm();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

private:
	UPROPERTY(EditDefaultsOnly)
		float HittedTime = 0.1f;//Collision 시 0.1마다 데미지

	UPROPERTY(EditDefaultsOnly)
		float DestoryTime = 5.0f;//유지시간

	UPROPERTY(EditDefaultsOnly)
		float Speed = 300.0f;//도는 빠르기

	UPROPERTY(EditDefaultsOnly)
		float Distance = 150.0f;//거리

private:
	UFUNCTION()
		void Finish();
	UFUNCTION()
		void Hitted();
private:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;


private:
	class UBoxComponent* Box;
	class UParticleSystemComponent* Attached;

	TArray<class ACharacter*> HittedCharacters;

	float Angle;
};
