#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	virtual void DoAction() override;
	virtual void DoAction(int InIndex) override;
public:
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Air_DoAction(bool InAirAction) override;
	virtual void LastAttack_DoAtction(bool InLastAttackAction) override;

	
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
public:
	void Press_Action() override;
	void Release_Action() override;
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

private:
	UFUNCTION()
		void RestoreGlobalDilation();
private:
	int32 Index = -1;

	bool bEnable;
	bool bExist;
	bool bLast;
	
	TArray<class ACharacter*> HittedCharacter;//연타방지
	bool bChargeSkillHit;
	bool bAirAction;
	bool bLastAttackAction;
};
