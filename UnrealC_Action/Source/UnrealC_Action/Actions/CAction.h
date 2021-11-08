#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAction.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;

};


USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* Effect_Niagara;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;
};

UCLASS()
class UNREALC_ACTION_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	friend class UCActionData;

public:
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

	FORCEINLINE class ACDoAction* GetBaseCombo() { return BaseCombo; }
	FORCEINLINE class ACDoAction* GetAirCombo() { return AirCombo; }
	FORCEINLINE class ACDoAction* GetSecondCombo() { return SecondCombo; }
	FORCEINLINE class ACDoAction* GetChargeCombo() { return ChargeCombo; }

	FORCEINLINE FLinearColor GetEquipmentColor() { return EquipmentColor; }
private:
	class ACAttachment* Attachment;
	class ACEquipment* Equipment;
	class ACDoAction* BaseCombo;
	class ACDoAction* AirCombo;
	class ACDoAction* SecondCombo;
	class ACDoAction* ChargeCombo;

	FLinearColor EquipmentColor;
};
