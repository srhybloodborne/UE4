#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnEquipmentDelegate);

UCLASS()
class UNREALC_ACTION_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEquipment();
protected:
	virtual void BeginPlay() override;
public:
	FORCEINLINE void SetData(FEquipmentData InData) { Data = InData; }
	FORCEINLINE void SetColor(FLinearColor InColor) { Color = InColor; }

	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }
public:
	//���� ���
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	//���� ����
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	//���� �Ϸ�
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();


	//���� ���
	UFUNCTION(BlueprintNativeEvent)
		void UnEquip();
	void UnEquip_Implementation();


protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;
	UPROPERTY(BlueprintAssignable)
		FUnEquipmentDelegate OnUnEquipmentDelegate;
private:
	bool bEquipped;

	FEquipmentData Data;
	FLinearColor Color;

};
