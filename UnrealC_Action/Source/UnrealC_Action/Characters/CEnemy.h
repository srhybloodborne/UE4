#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"
 


UCLASS()
class UNREALC_ACTION_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();
protected:
	virtual void BeginPlay() override;
public:
	virtual void ChangeColor(FLinearColor InColor) override;
	virtual void End_Dead() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
private://Delegate
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
public:
	FORCEINLINE void EnableNextHitted() { bEnable = true; }
	FORCEINLINE void DisableNextHitted() { bEnable = false; }
public:
	FORCEINLINE void CheckChargetSkillHitted(bool InChargeSkillHitted) { bChargeSkillHitted = InChargeSkillHitted; }
	FORCEINLINE void CheckAirAttackHitted(bool InAirHitted) { bAirHitted = InAirHitted; }
	FORCEINLINE void SetLastAttackHitted(bool InLastAttackHitted) { bLastAttackHitted = InLastAttackHitted; }
	FORCEINLINE void SetBowHitted(bool InBowHitted) { bBowHitted = InBowHitted; }

	FORCEINLINE class UBoxComponent* GetBox() { return Box; }
#pragma region Widget
//////////////////////////////////////////////////////////////////////////////////////////////////////
private://Widget
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* ExecutionWidget;
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region Component
//////////////////////////////////////////////////////////////////////////////////////////////////////
public://Actor Component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UCActionComponent* Action;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region Hitted
//////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 300.0f;
	UPROPERTY(EditDefaultsOnly)
		float AirLaunchValue = 800.0f;
private://Hitted
	class AController* DamageInstigator;
	float DamageValue;
private://Call by State
	void BaseHitted();
public://Call by Notify
	void Begin_BaseHitted();
	void End_BaseHitted();
private:
	UFUNCTION()
		void RestoreColor();
	void ChargeSkillHitted();
private:
	//ComboCheck
	bool bEnable;
	bool bExist;
	bool bLast;
	int Index = 0;

	//Enemy Hit Impulse
	FVector start;
	FVector target;
private://Air Attack Hit
	bool bAirHitted;
	void AirHitted();
	void End_AirHitted();
private://Last Attack Hit
	bool bLastAttackHitted;
private://Charge Attack Hit
	bool bChargeSkillHitted = false;
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
//Material
private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;
//Dead
private:
	void Dead();
private:
	void OnExecution();
	void Begin_Execution();
public:
	void End_Execution();
private:
	void OnExhausted();
	void Begin_Exhausted();
public:
	void End_Exhausted();
public:
	void Destroy_Attachment();
public:
	UFUNCTION()
		void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);//Execution Box BeginOverlap Envent
	UFUNCTION()
		void ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	bool bBowHitted;
	bool bCanExecution;
	class ACPlayer* player;

	bool bExhausted;
	int ExecutionCount = 0;
};
