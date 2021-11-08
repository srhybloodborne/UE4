// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Bow.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API ACDoAction_Bow : public ACDoAction
{
	GENERATED_BODY()

public:
	ACDoAction_Bow();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
public:
	virtual void DoAction() override;
	virtual void DoAction(int InIndex) override;

	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void OnAim() override;
	virtual void OffAim() override;

	virtual void Fire() override;

	virtual void Air_DoAction(bool InAirAction) override;
	virtual void LastAttack_DoAtction(bool InLastAttackAction) override;
public:
	void Press_Action() override;
	void Release_Action() override;
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }
	FORCEINLINE void EnableLaunchCharacter() { bLacunCharaacter = true; }
private:
	UFUNCTION()
		void RestoreGlobalDilation();
private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult);
private:
	//class UCAim* Aim;

	int32 Index = -1;

	bool bEnable;
	bool bExist;
	bool bLast;

	bool bLastAttackAction;
	bool bAirAttack;
	bool bLacunCharaacter;
	bool bChargeSkillHit;

	int Temp_Count = 700;
	int t_count = 0;
};
