#include "CPlayer.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraAnim.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/InputComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CTargetComponent.h"
#include "COmponents/CFeetComponent.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Widgets/CUserWidget_Select.h"
#include "Widgets/CUserWidget_WeaponWidget.h"
#include "Actions/CEquipment.h"

#include "Components/CapsuleComponent.h"


ACPlayer::ACPlayer()
{
 
	PrimaryActorTick.bCanEverTick = true;

	//Spring Arm
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = CameraLength;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	//Camera
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);
	Camera->SetRelativeLocation(FVector(0, 100, 0));
	//Skeleta Mesh
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//Anim Instance
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Feet, "Feet");
	CHelpers::CreateActorComponent(this, &Target, "Target");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &State, "State");

	//Movement Set
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Charge Camera Shake
	CHelpers::GetClass<UCameraShake>(&shake, "Blueprint'/Game/Actions/CameraShake/BP_CameraShake_Katana_Charge.BP_CameraShake_Katana_Charge_C'");
	
	//Widget
	CHelpers::GetClass<UCUserWidget_Select>(&SelectWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Select.WB_Select_C'");
	CHelpers::GetClass<UCUserWidget_WeaponWidget>(&WeaponWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_WeaponWidget.WB_WeaponWidget_C'");
	
	CHelpers::GetClass<UCameraShake>(&ExecutionShake, "Blueprint'/Game/Actions/CameraShake/BP_CameraShake_OneHand_3.BP_CameraShake_OneHand_3_C'");
	
	CHelpers::GetAsset<UCameraAnim>(&ExecutionCameraAnim_1, "CameraAnim'/Game/Actions/Camera/CA_Test00.CA_Test00'");
	CHelpers::GetAsset<UCameraAnim>(&ExecutionCameraAnim_2, "CameraAnim'/Game/Actions/Camera/CA_Test02.CA_Test02'");
	CHelpers::GetAsset<UCameraAnim>(&ExecutionCameraAnim_3, "CameraAnim'/Game/Actions/Camera/CA_Test03.CA_Test03'");
}

#pragma region Override
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ACPlayer::BeginPlay()
{
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/MI_Character_Body.MI_Character_Body'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/MI_Character_Logo.MI_Character_Logo'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);
	
	Super::BeginPlay();	//블프를 뒤에콜하려면 뒤에써야됨

	Action->SetUnarmedMode();

	SelectWidget = CreateWidget<UCUserWidget_Select, APlayerController>(GetController<APlayerController>(), SelectWidgetClass);
	SelectWidget->AddToViewport();
	SelectWidget->SetVisibility(ESlateVisibility::Hidden);

	SelectWidget->GetData(0).OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnKatana);
	SelectWidget->GetData(1).OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnAssassin);
	SelectWidget->GetData(2).OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnBow);
	//SelectWidget->GetData(3).OnUserWidget_Select_Clicked.AddDynamic(this, &ACPlayer::OnWarp);

	WeaponWidget = CreateWidget<UCUserWidget_WeaponWidget, APlayerController>(GetController<APlayerController>(), WeaponWidgetClass);
	WeaponWidget->AddToViewport();
	WeaponWidget->SetVisibility(ESlateVisibility::Visible);
	
	Action->OnActionTypeChanged.AddDynamic(WeaponWidget, &UCUserWidget_WeaponWidget::ChangeWeapon);

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	//Charge Event
	if (bCharge)
	{
		SpringArm->TargetArmLength = SpringArm->TargetArmLength - 0.2f;
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 200.0f, 680.0f);
		float scale = ChargeCount * 0.1f;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake,scale);
		ChargeTime = ChargeTime + UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		if (ChargeTime >= 1.3f)
		{
			if (ChargeCount <= 3)
			{
				ChargeTime = 0.0f;
				ChargeCount = ChargeCount + 1;
				FMath::Clamp(ChargeCount, 0, 3);
				SpringArm->TargetArmLength = SpringArm->TargetArmLength - ChargeCount * 80.0f;
				SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 200.0f, 680.0f);
			}
		}
	}
	else
	{
		if (!FMath::IsNearlyEqual(SpringArm->TargetArmLength, CameraLength))
		{
			float temp = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength,CameraLength,DeltaTime, InteropSpeed);
			SpringArm->TargetArmLength = (temp);
		}
		ChargeTime = 0.0f;
		ChargeCount = 0;
	}

	//ChangeColor(FLinearColor(1, 1, 1, 1));

	//if (State->IsIdleMode() == false)
	//	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	//if (State->IsIdleMode())
	//	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OffWalk);
	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);
	PlayerInputComponent->BindAction("Guard", EInputEvent::IE_Pressed, this, &ACPlayer::OnGuard);
	PlayerInputComponent->BindAction("Execution", EInputEvent::IE_Pressed, this, &ACPlayer::OnExecution);

	PlayerInputComponent->BindAction("Katana", EInputEvent::IE_Pressed, this, &ACPlayer::OnKatana);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("Iceball", EInputEvent::IE_Pressed, this, &ACPlayer::OnIceball);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, this, &ACPlayer::OnBow);
	PlayerInputComponent->BindAction("Assassin", EInputEvent::IE_Pressed, this, &ACPlayer::OnAssassin);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("FireStorm", EInputEvent::IE_Pressed, this, &ACPlayer::OnFireStorm);

	PlayerInputComponent->BindAction("BaseCombo", EInputEvent::IE_Pressed, this, &ACPlayer::OnBaseCombo);
	PlayerInputComponent->BindAction("AirCombo", EInputEvent::IE_Pressed, this, &ACPlayer::OnAirCombo);
	PlayerInputComponent->BindAction("SecondCombo", EInputEvent::IE_Pressed, this, &ACPlayer::OnSecondCombo);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("Charge", EInputEvent::IE_Pressed, this, &ACPlayer::OnCharge);
	PlayerInputComponent->BindAction("Charge", EInputEvent::IE_Released, this, &ACPlayer::OffCharge);

	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, this, &ACPlayer::OnTarget);
	PlayerInputComponent->BindAction("TargetLeft", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetLeft);
	PlayerInputComponent->BindAction("TargetRight", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetRight);


	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSelectAction);
	PlayerInputComponent->BindAction("SelectAction", EInputEvent::IE_Released, this, &ACPlayer::OffSelectAction);

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion


//Delegate
/// <summary>
/// State-> Set <State> 시 Broadcast되는 Delegate
/// 따라서 State만 바꿔도 실제 동작함수 호출가능
/// State : Roll, BackStep => Montage ->Play Anim
/// </summary>
void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Idle:
		break;
	case EStateType::Roll:
		Begin_Roll();
		break;
	case EStateType::Backstep:
		Begin_Backstep();
		break;
	case EStateType::Equip:
		break;
	case EStateType::Action:
		break;
	case EStateType::Max:
		break;
	case EStateType::Guard:
		Begin_Guard();
		break;
	case EStateType::Execution:
		Begin_Execution();
		break;
	case EStateType::Hitted:
		BaseHitted();
		break;
	default:
		break;
	}
}

#pragma region Camera Event
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ACPlayer::OnHorizontalLook(float InAxis)
{
	CheckTrue(State->IsExecutionMode());
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	CheckTrue(State->IsExecutionMode());
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoom(float InAxis)
{
	SpringArm->TargetArmLength += (ZoomSpeed * InAxis * GetWorld()->GetDeltaSeconds());
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, ZoomRange.X, ZoomRange.Y);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region Move Evnet
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Axis Bind
void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}
void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

//Action Bind
void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
}
void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region State Event
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Call By Input Press
void ACPlayer::OnAvoid()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->CanMove());

	if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
	{
		State->SetBackstepMode();

		return;
	}

	State->SetRollMode();
}
void ACPlayer::OnGuard()
{
	CheckFalse(State->IsIdleMode());
	State->SetGuaradMode();
}
void ACPlayer::OnExecution()
{
	if (bCanExecution == true)
	{
		CheckFalse(State->IsIdleMode());
		if (Action->IsKatanaMode() == false)
		{
			OnKatana();
			Action->GetCurrent()->GetEquipment()->Begin_Equip();
			Action->GetCurrent()->GetEquipment()->End_Equip();
		}
		ExecutionCount = UKismetMathLibrary::RandomIntegerInRange(0, 2);

		WeaponWidget->SetVisibility(ESlateVisibility::Hidden);
		State->SetExecutionMode();
	}
}
//Call By Delegate
void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackstep();
}
void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector end = start + GetVelocity().GetSafeNormal2D();

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, end));
	Montages->PlayRoll();
}
void ACPlayer::Begin_Guard()
{
	Status->SetStop();
	Montages->PlayGuard();
}
void ACPlayer::Begin_Execution()
{
	Status->SetStop();
	FRotator rotation;
	switch (ExecutionCount)
	{
	case 0:
		Montages->PlayExecution();
		rotation = FRotator(0, UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation().Yaw, 0);
		GetController()->SetControlRotation(rotation);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientPlayCameraAnim(ExecutionCameraAnim_1, 1.0f, 1.0f, 0.2f, 0.3);
		break;
	case 1:
		Montages->PlayExecution2();
		rotation = FRotator(0, UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation().Yaw, 0);
		GetController()->SetControlRotation(rotation);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientPlayCameraAnim(ExecutionCameraAnim_2, 1.0f, 1.0f, 0.2f, 0.3);
		break;
	case 2:
		Montages->PlayExecution3();
		rotation = FRotator(0, UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation().Yaw, 0);
		GetController()->SetControlRotation(rotation);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ClientPlayCameraAnim(ExecutionCameraAnim_3, 1.0f, 1.0f, 0.2f, 0.3);
		break;
	}
}

//Call By Notify
void ACPlayer::End_Backstep()
{
	if (Action->IsUnarmedMode() == true)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	State->SetIdleMode();
}
void ACPlayer::End_Roll()
{
	if (Action->IsUnarmedMode() == false)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	State->SetIdleMode();
}
void ACPlayer::End_Guard()
{
	Status->SetMove();
	State->SetIdleMode();
}
void ACPlayer::End_Execution()
{
	Status->SetMove();
	State->SetIdleMode();

	WeaponWidget->SetVisibility(ESlateVisibility::Visible);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion


#pragma region Katana Event
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ACPlayer::OnKatana()
{
	CheckFalse(State->IsIdleMode());
	Action->SetKatanaMode();
}
void ACPlayer::OnBaseCombo()
{
	Action->BaseCombo();
}
void ACPlayer::OnAirCombo()
{

	Action->AirCombo();
}
void ACPlayer::OnSecondCombo()
{
	Action->SecondCombo();
}
void ACPlayer::OnCharge()
{
	bCharge = true;

	CameraInitialLocation = Camera->GetRelativeLocation();
	SpringArm->TargetArmLength = 650.0f;
	Camera->SetRelativeLocation(FVector::ZeroVector);
	Status->SetStop();

	Action->ChargeComboPress();
}
void ACPlayer::OffCharge()
{
	bCharge = false;

	//자연스럽게 원상복구rinto
	SpringArm->TargetArmLength = 1500.0f;
	Camera->SetRelativeLocation(CameraInitialLocation);
	//Status->SetMove();
	//State->IsIdleMode();
	//StopAnimMontage();
	Action->ChargeComboRelease();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion

//Two Hand
void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());
	Action->SetTwoHandMode();
}

//Fist
void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());
	Action->SetFistMode();
}

//Iceball
void ACPlayer::OnIceball()
{
	CheckFalse(State->IsIdleMode());
	Action->SetIceballMode();
}


void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());
	Action->SetWarpMode();
}

void ACPlayer::OnFireStorm()
{
	CheckFalse(State->IsIdleMode());
	Action->SetFireStormMode();
}


void ACPlayer::OnBow()
{
	CheckFalse(State->IsIdleMode());
	Action->SetBowMode();
}

void ACPlayer::OnAim()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//Action->DoAim(true);
}

void ACPlayer::OffAim()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//Action->DoAim(false);
}



void ACPlayer::OnAssassin()
{
	CheckFalse(State->IsIdleMode());
	Action->SetAssassinMode();
}

#pragma region Target Event
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ACPlayer::OnTarget()
{
	Target->ToggleTarget();
}
void ACPlayer::OnTargetLeft()
{
	Target->ChangeTarget(false);
}
void ACPlayer::OnTargetRight()
{
	Target->ChangeTarget(true);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion



void ACPlayer::OnSelectAction()
{
	CheckFalse(State->IsIdleMode());

	SelectWidget->SetVisibility(ESlateVisibility::Visible);
	GetController<APlayerController>()->bShowMouseCursor = true;
	//GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffSelectAction()
{
	CheckFalse(State->IsIdleMode());

	SelectWidget->SetVisibility(ESlateVisibility::Hidden);
	GetController<APlayerController>()->bShowMouseCursor = false;
	//GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void ACPlayer::ExecutionCameraShake()
{
	if (!!ExecutionShake)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(ExecutionShake);
	}
}

float ACPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//CLog::Print("Check");
	if (State->IsIdleMode() == false)
		return 0;

	//CLog::Print(Damage);
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (State->IsExhaustedMode())
		return Status->GetHealth();


	
	State->SetHittedMode();
	

	return Status->GetHealth();
}


void ACPlayer::RestoreColorPlayer()
{
	//CLog::Print("Check");
	//	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();
	FLinearColor color = FLinearColor::White;

	ChangeColor(color);
}
// Base Attack Hit
void ACPlayer::BaseHitted()
{
	//If Next Combo Enable : return
	Status->CanMove();
	//Health Sub
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;
	
	//Base Combo Hit	
	Montages->PlayHitted();
	
	//Enemy Hit Impulse
	FVector start;
	FVector target;

	//Rotator to Hitter
	start = GetActorLocation();
	target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	//Impulse Move
	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * 300, true, false);
		

	//Color Change
	ChangeColor(FLinearColor(1, 0, 0, 1));
	FTimerHandle UnusedHandle;
	float Duration = 0.1f;
	bool bLoop = false;

	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ACPlayer::RestoreColorPlayer, Duration, bLoop);
}


void ACPlayer::End_BaseHitted()
{

	State->SetIdleMode();

}


/*
* Montage Component
* FMontage Datas[EStateType]
*/

/*
* Action Component
* ActionData Datas[EActionType]
*	- AttachmentClass
*	- EquipmentClass
*	- EquipmentData - Montage,PlayRatio,StartSection,~
*	- DoActionClass
*	- DoActionData -~~~~~~~~~~~
*/




// 1. Roll, BackStep
/*
* Roll, BackStep
* 사전 처리
* 1. DataTable 작성 : Csv
* 2. Import DataTable Asset
* 3. BP_Player -> Montage Component -> DataTable 할당 (BpDefaultOnly)
* 
* 실제 동작
* 1. In Game Action Press ->On Avoid 
* 2. State Component -> Set State Roll -> Begin Roll(Delegate)
* 3. Montage Play Anim
* 4. End Call by Notify
*/

//2. Equip Sword Animation
/* 
* Equip Sword (Animation)
* 사전 처리
* 1. Data Asset 생성
* 2. 부모 CActionData
* 3. Property 3개 
*	(1) EquipmentClass
*	(2) EquipmentData
*		AnimMontage, PlayRatio, StartSection, CanMove, PawnControl
*	(3) EquipmentColor
* 4. Property 3개 할당
* 5. BP_CPlayer -> Action Component -> Datas 에 만든 Data Asset(Action Data) 할당
* 
* 실제 동작
* Keyward : Equipment (Player-Action Component-Datas(ActionDatas)-Equipment 
* 1. In Game Action Press ->On Katana
* 2. ActionComponent -> Set Mode (One Hand), Change Type : Action Type 바꿈(Delegation 아직 Bind된거 없음)
* 3.  Set Mode -> Datas[Katana] -> GetEquipment -> Equip -> Play Anim(Data.anim,~~) -> End Eqip Call By Notify
*/


//3. Equip Sword Attachment
/*
* Equip Sword (Attachment)
* 사전 처리
* Data Asset
* 1. Data Asset 생성
* 2. 부모 CActionData
* 3. AttachmentClass - BP_Attachment_Katana
* 5. BP_CPlayer -> Action Component -> Datas 에 만든 Data Asset(Action Data) 할당
*
* Add Dynamic Delegate
* 1. Delcare Delegate : OnEquip,OnUnEquip -> Equipmentclass
* 2. Add Dyanmic : Action Data - BeginPlay - if(Attachment) AddDynamic : Attachment's OnEquip, UnEquip
* 3. BroadCast : Equipmentclass - Begin Equip, End Equip
* 
* 
* 실제 동작
* 1. Spawn
* ActionComponent - Datas[Katana] - AttachmentClass - Begin Play - OwnerCharacter(Player) - SpawnActor(this)
* 2.Begin Equip
* Equipment - Equip - Play Anim Montage - AnimNotify - Begin Equip (Delegate) - Attachment's On Equip - Attach To(BP)
* Equipment - UnEquip - Play Anim Montage - AnimNotify - Un Equip (Delegate) - Attachment's ON UnEquip - Attach To(BP)
*/


//4. Attack Sword Aniamtion
/*
* Attack Sword (Animation)
* 사전 처리
* Data Asset
* 1. Data Asset 생성
* 2. 부모 CActionData
* 3. DoActionClass - DoActionMelee
* 5. BP_CPlayer -> Action Component -> Datas 에 만든 Data Asset(Action Data) 할당
*
* 실제 동작
* Player - Action Press - AcionComponent - GetBaseCombo - Datas[FActionData] - Play Anim Montage
*/

//5. Enemy Take Damage
/*
* 사전 처리
* 1. Delegate Delcare
*	Attachment :: OnAttachmentBeginOverlap
* 2. Bind될 함수 
*	DoAction :: OnAttachmentBeginOverlap 함수이름와 매개변수 리턴값 모두같지만 다른함수
*	내용 : Enemy -> TakeDamage
* 3. Bind될 위치
*	ActionData 
*	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, DoAction::OnAttachmentBeginOverlap)
* 4. Broadcast될 함수
*	Attachment :: ComponentBeginOverlap (내꺼)
* 5. Broadcast될 위치
*	Attachment
*	component(Collision들의 부모, Attachment필드)->OnComponentBeginOverlap(진짜 충돌, 언리얼 다이나믹 델리게이션).AddDynamic( Attachment::ComponentBeginOverlap (내꺼)
*	
* 실제동작
*  . Attack Sword Aniamtion -> Animnotify State Collision -	Attachment On Collision - component->OnComponentBeginOverlap (실제충돌발생)
*  - OnAttachmentBeginOverlap Broadcast - Enemy :: Take Damage
*/




