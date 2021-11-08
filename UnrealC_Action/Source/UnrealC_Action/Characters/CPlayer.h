#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Characters/ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class UNREALC_ACTION_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	ACPlayer();
//Override
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
	virtual void ChangeColor(FLinearColor InColor) override;
//Delegate
private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
private:
	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
		float ZoomSpeed = 1000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
		FVector2D ZoomRange = FVector2D(0,500);
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 0;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_Select> SelectWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_WeaponWidget> WeaponWidgetClass;
#pragma region Component
//////////////////////////////////////////////////////////////////////////////////////////////////////
private://Scene Component
	UPROPERTY(VisibleInstanceOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleInstanceOnly)
		class UCameraComponent* Camera;

private://Actor Component
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UCStateComponent* State;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UCActionComponent* Action;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetComponent* Target;
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

public:
	FORCEINLINE class UCUserWidget_Select* GetSelectWidget() { return SelectWidget; }
	FORCEINLINE void SetCanExecution(bool InCanExecution) { bCanExecution = InCanExecution; }
	FORCEINLINE int GetExecutionCount() { return ExecutionCount; }
	FORCEINLINE class UCStateComponent* GetState() { return State; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
//Camera Event
//////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);

//////////////////////////////////////////////////////////////////////////////////////////////////////

//Move Event
//////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnWalk();
	void OffWalk();
//////////////////////////////////////////////////////////////////////////////////////////////////////

//State Event
//////////////////////////////////////////////////////////////////////////////////////////////////////
private://Call by Input Button
	void OnAvoid();
	void OnGuard();
	void OnExecution();
private:
	void Begin_Backstep();
	void Begin_Roll();
	void Begin_Guard();
	void Begin_Execution();
public://Call by Notify
	void End_Backstep();
	void End_Roll();
	void End_Guard();
	void End_Execution();
//////////////////////////////////////////////////////////////////////////////////////////////////////

//Katana Event
private:
	UFUNCTION()
		void OnKatana();
private://Attack Combo	
	void OnBaseCombo();
	void OnAirCombo();
	void OnSecondCombo();
private://Charge
	void OnCharge();
	void OffCharge();

//Two Hand
private:
	UFUNCTION()
		void OnTwoHand();
//Ice Ball
private:
	void OnIceball();
private:
	UFUNCTION()
		void OnBow();
	void OnAim();
	void OffAim();
//Fist
private:
	void OnFist();
private:
	UFUNCTION()
		void OnAssassin();
private:
	UFUNCTION()
		void OnWarp();
private:
	UFUNCTION()
		void OnFireStorm();
//Target Event
private:
	void OnTarget();
	void OnTargetLeft();
	void OnTargetRight();

//Select Event
private:
	void OnSelectAction();
	void OffSelectAction();
//Variable
private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;
	class UCUserWidget_Select* SelectWidget;
	class UCUserWidget_WeaponWidget* WeaponWidget;
	
public:
	void ExecutionCameraShake();
private:
	TSubclassOf<class UCameraShake> ExecutionShake;
	class UCameraAnim* ExecutionCameraAnim_1;
	class UCameraAnim* ExecutionCameraAnim_2;
	class UCameraAnim* ExecutionCameraAnim_3;
	bool bCanExecution;
	int ExecutionCount = 0;
public:
//Charge
//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool bCharge;
	FVector CameraInitialLocation;
	float ChargeTime;
	int ChargeCount = 1;
	TSubclassOf<UCameraShake> shake;
	float InteropSpeed = 0.75f;
	float CameraLength = 350.0f;
//////////////////////////////////////////////////////////////////////////////////////////////////////
private://Hitted
		class AController* DamageInstigator;
		float DamageValue;
private://Call by State
	void BaseHitted();
public://Call by Notify
	void RestoreColorPlayer();
	void End_BaseHitted();
};
 