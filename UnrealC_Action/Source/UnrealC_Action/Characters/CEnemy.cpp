#include "CEnemy.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "COmponents/CFeetComponent.h"
#include "Components/CMontagesComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Widgets/CUserWidget_Execution.h"
#include "Components/BoxComponent.h"
#include "Characters/CPlayer.h"
#include "Components/CapsuleComponent.h"


ACEnemy::ACEnemy()
{
	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &ExecutionWidget, "ExecutionWidget", GetMesh());
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", GetMesh());
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Feet, "Feet");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	Box->SetRelativeLocation(FVector(0, 0, 120));
	Box->SetRelativeScale3D(FVector(8, 8, 3));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemies/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();


	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NameWidget->SetVisibility(false);

	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Execution> executionClass;
	CHelpers::GetClass<UCUserWidget_Execution>(&executionClass, "WidgetBlueprint'/Game/Widgets/WB_Execution.WB_Execution_C'");
	ExecutionWidget->SetWidgetClass(executionClass);
	ExecutionWidget->SetRelativeLocation(FVector(90, 0, 0));
	ExecutionWidget->SetDrawSize(FVector2D(64, 64));
	ExecutionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ExecutionWidget->SetVisibility(false);

	Box->SetGenerateOverlapEvents(false);
	//Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->bHiddenInGame = true;

}

void ACEnemy::BeginPlay()
{
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/MI_Character_Body.MI_Character_Body'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/MI_Character_Logo.MI_Character_Logo'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACEnemy::ComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACEnemy::ComponentEndOverlap);


	Super::BeginPlay();	
	//Action->SetUnarmedMode();

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetName());
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetControllerText(GetController()->GetName());
	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());	
	ExecutionWidget->InitWidget();
	ExecutionWidget->SetVisibility(false);

	bCanExecution = false;
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CLog::Print(bCanExecution);

	if (Status->GetHealth() <= 20.0f)
	{

		if (bExhausted == false)
		{
			Box->SetGenerateOverlapEvents(true);
			OnExhausted();
		}
		//Box->bHiddenInGame = false;
	}

	if (bCanExecution == true)
	{
		if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::F))
		{
			OnExecution();
		}
	}
}



void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Idle:
		break;
	case EStateType::Roll:
		break;
	case EStateType::Backstep:
		break;
	case EStateType::Equip:
		break;
	case EStateType::Action:
		break;
	case EStateType::Hitted:
		BaseHitted();
		break;
	case EStateType::ChargeSkillHitted:
		ChargeSkillHitted();
		break;
	case EStateType::AirHitted:
		AirHitted();
		break;
	case EStateType::Dead:
		Dead();
		break;
	case EStateType::Execution:
		Begin_Execution();
		break;
	case EStateType::Exhausted:
		Begin_Exhausted();
		break;
	case EStateType::Max:
		break;
	default:
		break;
	}
}
#pragma region Hitted
//////////////////////////////////////////////////////////////////////////////////////////////////////
float ACEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//CLog::Print(Damage);
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (State->IsExhaustedMode())
		return Status->GetHealth();


	if (bChargeSkillHitted)
	{
		State->SetChargeSkillHitted();
	}
	else if (bAirHitted)
	{
		State->SetAirHittedMode();
	}
	else
	{
		State->SetHittedMode();
	}

	return Status->GetHealth();
}
void ACEnemy::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}
void ACEnemy::RestoreColor()
{
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();

	ChangeColor(color);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Base Attack Hit
void ACEnemy::BaseHitted()
{
	//If Next Combo Enable : return
	if (bEnable == true)
	{
		bExist = true;
		bEnable = false;

		return;
	}
	
	//Health Sub
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	//If Dead
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return;
	}

	//If Last Attack Hitted
	if (bLastAttackHitted == true)
	{
		StopAnimMontage();
		
		Montages->PlayLastBaseAttackHitted();
		Index = 0;
	}//Base Combo Hit
	else if (Index == 0)
		Montages->PlayHitted();
	else if (Index == 1)
		Montages->PlayHitted2();
	else if (Index == 2)
		Montages->PlayHitted3();
	else
	{
		Montages->PlayHitted();
		Index = 0;
	}
	
	//Etc
	{
		if (bBowHitted == false)
		{
			//Rotator to Hitter
			start = GetActorLocation();
			target = DamageInstigator->GetPawn()->GetActorLocation();
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
			DamageInstigator = NULL;

			//Impulse Move
			FVector direction = target - start;
			direction.Normalize();
			LaunchCharacter(-direction * LaunchValue, true, false);
		}
	
		//Color Change
		ChangeColor(FLinearColor(1, 0, 0, 1));
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);

		Index++;
	}
}

void ACEnemy::Begin_BaseHitted()
{
	//¾È¾¸
	/*
	//CheckFalse(bExist);
	//bExist = false;

	//Status->SubHealth(DamageValue);
	//DamageValue = 0.0f;

	//Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	////this->StopAnimMontage();

	//Index++;
	//if (Index == 1)
	//{

	//	Montages->PlayHitted2();
	//	FVector direction = target - start;
	//	direction.Normalize();
	//	LaunchCharacter(-direction * LaunchValue, true, false);

	//	ChangeColor(FLinearColor(1, 0, 0, 1));
	//	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);
	//}
	//else if (Index == 2)
	//{
	//	Montages->PlayHitted3();
	//	FVector direction = target - start;
	//	direction.Normalize();
	//	LaunchCharacter(-direction * LaunchValue, true, false);

	//	ChangeColor(FLinearColor(1, 0, 0, 1));
	//	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);
	//}
	*/
}

void ACEnemy::End_BaseHitted()
{
	//this->StopAnimMontage();
	bLastAttackHitted = false;
	bAirHitted = false;
	State->SetIdleMode();
	start = FVector::ZeroVector;
	target = FVector::ZeroVector;
	bBowHitted = false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Air Attack Hit
void ACEnemy::AirHitted()
{
	//Health Sub
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	//If Dead
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return;
	}

	//If Last Attack Hitted 
	if (bLastAttackHitted == true)
	{
		StopAnimMontage();
		Montages->PlayLastAirAttackHitted();
	}
	//Actual Play
	else
	{
		Montages->PlayAirHitted();
	}
	
	//Etc
	{
		if (bBowHitted == false)
		{
			//Rotator to Hitter
			start = GetActorLocation();
			target = DamageInstigator->GetPawn()->GetActorLocation();
			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
			DamageInstigator = NULL;

			//Impulse Move
			FVector direction = target - start;
			direction.Normalize();
			LaunchCharacter(-direction * AirLaunchValue, true, false);
		}
		//Color Change
		ChangeColor(FLinearColor(1, 0, 0, 1));
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);
	}
}
void ACEnemy::End_AirHitted()
{
	//this->StopAnimMontage();
	State->SetIdleMode();
	start = FVector::ZeroVector;
	target = FVector::ZeroVector;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Charge Attack Hit
void ACEnemy::ChargeSkillHitted()
{
	//Health Sub
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	
	//If Dead
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return;
	}

	//Etc
	{
		//Rotator to Hitter
		start = GetActorLocation();
		target = DamageInstigator->GetPawn()->GetActorLocation();
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		DamageInstigator = NULL;

		//Impulse Move
		FVector direction = target - start;
		direction.Normalize();
		LaunchCharacter(-direction * LaunchValue, true, false);

		//Color Change
		ChangeColor(FLinearColor(1, 0, 0, 1));
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);
	}

	//Actual Play
	Montages->PlayChargeSkillHitted();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

void ACEnemy::Dead() 
{
	CheckFalse(State->IsDeadMode());


	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Action->Dead();

	Montages->PlayDead();
}	

void ACEnemy::Destroy_Attachment()
{
	//Action->DestroyComponent();
	Action->End_Dead();
}

void ACEnemy::End_Dead()
{
	this->Destroy();
}

void ACEnemy::OnExecution()
{
	//CLog::Print("Execution");
	State->SetExecutionMode();
}

void ACEnemy::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPlayer* temp_player = Cast<ACPlayer>(OtherActor);
	CheckFalse(temp_player);
	if (!!temp_player)
	{

			player = temp_player;
			player->SetCanExecution(true);
			//CLog::Print("ComponentBegin Overlap");
			ExecutionWidget->SetVisibility(true);
			bCanExecution = true;
	}
}

void ACEnemy::ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPlayer* temp_player = Cast<ACPlayer>(OtherActor);
	
	CheckFalse(temp_player);
	if (!!temp_player)
	{
		
			//CLog::Print("ComponentBegin End");
			player->SetCanExecution(false);
			ExecutionWidget->SetVisibility(false);
			bCanExecution = false;
			player = nullptr;
			//ExecutionCount = -1;
	}

}

void ACEnemy::Begin_Execution()
{
	if (player == nullptr)
	{
		CLog::Print("Error Occurred Player didn't Exist then Excuted");
		return;
	}
	//State->SetExecutionMode();
	Status->SetStop();
	ExecutionCount = player->GetExecutionCount();

	StopAnimMontage();
	Box->SetGenerateOverlapEvents(false);
	ExecutionWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);
	NameWidget->SetVisibility(false);
	bCanExecution = false;

	FVector location = player->GetActorLocation() + player->GetActorForwardVector() * 300;
	FRotator rotation = player->GetActorRotation() + FRotator(0,180,0);
	//FRotator rotation = player->GetActorRotation() + FRotator(0, 180, 0);

	SetActorLocation(location);
	SetActorRotation(FQuat(rotation));

	//CLog::Print(ExecutionCount);

	switch (ExecutionCount)
	{
	case 0:
		Montages->PlayExecution();
		break;
	case 1:
		Montages->PlayExecution2();
		break;
	case 2:
		Montages->PlayExecution3();
		break;
	}
}

void ACEnemy::End_Execution()
{
	State->SetDeadMode();;
}


void ACEnemy::OnExhausted()
{
	State->SetExhaustedMode();
}

void ACEnemy::Begin_Exhausted()
{
	//Cast<UPrimitiveComponent>(GetCapsuleComponent())->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Montages->PlayExhausted();
	bExhausted = true;

}

void ACEnemy::End_Exhausted()
{
	//Cast<UPrimitiveComponent>(GetCapsuleComponent())->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	State->SetIdleMode();
	Box->SetGenerateOverlapEvents(false);
}

