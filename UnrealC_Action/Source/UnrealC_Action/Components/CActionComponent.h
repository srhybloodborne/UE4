#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"


UENUM(BlueprintType)
enum class EActionType: uint8
{
	Unarmed, Fist, Katana, Scabarrd, TwoHand, Warp, FireStorm, Iceball, Bow, Quiver, Assassin, Assassin_Scabard, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALC_ACTION_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCActionComponent();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* DataAssets[(int32)EActionType::Max];
public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetCurrent() { return Datas[(int32)Type]; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsKatanaMode() { return Type == EActionType::Katana; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsScabarrdMode() { return Type == EActionType::Scabarrd; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFireStormMode() { return Type == EActionType::FireStorm; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIceballMode() { return Type == EActionType::Iceball; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBowMode() { return Type == EActionType::Bow; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsQuiverMode() { return Type == EActionType::Quiver; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAssassinMode() { return Type == EActionType::Assassin; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAssassinScabardMode() { return Type == EActionType::Assassin_Scabard; }
public:
	UFUNCTION(BlueprintCallable)
		void SetUnarmedMode();

	void SetFistMode();

	UFUNCTION(BlueprintCallable)
		void SetKatanaMode();

	void SetScabarrdMode();

	UFUNCTION(BlueprintCallable)
		void SetTwoHandMode();

	void SetFireStormMode();
	void SetWarpMode();

	UFUNCTION(BlueprintCallable)
		void SetIceballMode();

	UFUNCTION(BlueprintCallable)
		void SetBowMode();
	void SetQuiverMode();
	void SetAssassinMode();
	void SetAssassinScabardMode();
public:
	void BaseCombo();
	void BaseCombo(int Index);
	void AirCombo();
	void ChargeComboPress();
	void ChargeComboRelease();
	void SecondCombo();
	
	void DoAim(bool InAim);
public:
	void OffAllCollision();
	void OnAllCollision();
private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);
public:
	void Damaged();
	void Dead();
	void End_Dead();

private:
	EActionType Type;
	class UCAction* Datas[(int32)EActionType::Max];
};
