
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class UNREALC_ACTION_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoAction();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
public:
	virtual void DoAction() {}
	virtual void DoAction(int InIndex) {}
	virtual void Begin_DoAction() {}//Call by Notify
	virtual void End_DoAction() {}//Call by Notify
	virtual void Press_Action() {}
	virtual void Release_Action() {}

	virtual void Air_DoAction(bool InAirAction) {}
	virtual void LastAttack_DoAtction(bool InLastAttackAction) {}

	virtual void Avort() {}
	
	/// <summary>
	/// ////////////////////////////////////////////////
	/// Arrow
	/// </summary>
	virtual void OnAim() {}
	virtual void OffAim() {}

	virtual void Fire() {}

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}

public:
	FORCEINLINE void SetDatas(TArray<FDoActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;
protected:
	TArray<FDoActionData> Datas;
	const bool* bEquipped;
};
