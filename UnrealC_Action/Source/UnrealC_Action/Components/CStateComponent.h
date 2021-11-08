#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Roll, Backstep, Equip, Action, Hitted, Charge, Guard, ChargeSkillHitted, Hitted2, Hitted3, AirHitted, LastBaseAttackHitted, LastAirAttackHitted, Dead, Execution, Execution2, Execution3, Exhausted, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALC_ACTION_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCStateComponent();
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBackstepMode() { return Type == EStateType::Backstep; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsChargeMode() { return Type == EStateType::Charge; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsGuardMode() { return Type == EStateType::Guard; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAirHitted() { return Type == EStateType::AirHitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsExecutionMode() { return Type == EStateType::Execution; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsExhaustedMode() { return Type == EStateType::Exhausted; }

// Enemy Hit
///////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsChargeSkillHitted() { return Type == EStateType::ChargeSkillHitted; }
public:
	void SetIdleMode();
	void SetRollMode();
	void SetBackstepMode();
	void SetEquipMode();
	void SetActionMode();
	void SetHittedMode();
	void SetChargeMode();
	void SetGuaradMode();
	void SetChargeSkillHitted();
	void SetAirHittedMode();
	void SetDeadMode();
	void SetExecutionMode();
	void SetExhaustedMode();
	
	void ChangeType(EStateType InNewType);

public:
	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;
private:
	EStateType Type;
};
