#include "CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CStateComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//생성시 파티클 가져옴
	CHelpers::GetAsset<UParticleSystem>(&Particle, "ParticleSystem'/Game/Particles/P_Enrage_Base.P_Enrage_Base'");
}

void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	//OwnerCharacter - Player
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckNull(Target);
	
	//틱마다 Target이 NULL이 아니라면 Target의 state를 가져옴
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);

	//거리체크
	bool b = false;
	b |= state->IsDeadMode();
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;

	if (b == true)
	{
		EndTargeting();

		return;
	}

	//Player로부터 Target의 회전값 구하기
	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	//DeltaTIme마다 InteropSpeed값에 따라 Player to Target에 대한 방향값 회전값 보간
	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InteropSpeed);
	OwnerCharacter->GetController()->SetControlRotation(rotator);//해당값으로 직접회전
}

//호출 순서 1.
void UCTargetComponent::ToggleTarget()
{
	if (!!Target)
	{
		EndTargeting();

		return;
	}

	StartTargeting();
}

void UCTargetComponent::StartTargeting()
{
	SetTraceTargets();
	SetTarget();
}

void UCTargetComponent::EndTargeting()
{
	Target = NULL;
	TraceTargets.Empty();

	if (!!Attached)
		Attached->DestroyComponent();
}

//호출 순서 2.
void UCTargetComponent::SetTraceTargets()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	//충돌로 검출 TraceRadius만큼 그리고 TraceTargets에 저장(중복제외)
	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius, "Pawn", false, ignoreActors, DebugType, hitResults, true, FLinearColor::Green, FLinearColor::Red, 1.0f);
	for (const FHitResult& hitResult : hitResults)
	{
		if (hitResult.GetActor()->GetClass() == OwnerCharacter->GetClass())
			continue;

		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
		if (!!character)
			TraceTargets.AddUnique(character);
	}
	//for (ACharacter* character : TraceTargets)
		//CLog::Print(character->GetActorLabel());
}

//호출 순서3.
void UCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* target = NULL;

	for (ACharacter* character : TraceTargets)
	{
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();//Player 컨트롤러의 전방백터
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();//Player로부터 지금 충돌Pawn으로 향하는 방향벡터
		offset = offset.GetSafeNormal();
		float temp = direction | offset;//Player의 전방벡터와, Player to 현재 Pawn의 백터의 내적

		if (temp < angle)//temp값이 제일큰놈 찾는거 즉 각도가 0에 가까운놈 찾는거 
			continue;

		angle = temp;
		target = character;
	}

	ChangeCursor(target);
}

void UCTargetComponent::ChangeCursor(ACharacter* InCharacter)
{
	if (!!InCharacter)
	{
		if (!!Attached)
			Attached->DestroyComponent();

		Attached = UGameplayStatics::SpawnEmitterAttached(Particle, InCharacter->GetMesh(), "Spine_Target");
		Target = InCharacter;

		return;
	}

	EndTargeting();
}

void UCTargetComponent::ChangeTarget(bool InRight)
{
	CheckNull(Target);

	TMap<float, ACharacter*> map;
	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue;

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		FVector ownerToTaget = targetLocation - ownerLocation;

		FQuat quat = FQuat(OwnerCharacter->GetControlRotation());
		FVector forward = quat.GetForwardVector();
		FVector up = quat.GetUpVector();

		FVector cross = forward ^ ownerToTaget;
		float dot = cross | up;

		map.Add(dot, character);
	}


	float minimum = FLT_MAX;
	ACharacter* target = NULL;

	TArray<float> keys;
	map.GetKeys(keys);
	for (float key : keys)
	{
		if (InRight == true)
		{
			if (key < 0.0f)
				continue;
		}
		else
		{
			if (key > 0.0f)
				continue;
		}

		if (FMath::Abs(key) > minimum)
			continue;

		target = *map.Find(key);
		minimum = FMath::Abs(key);
	}

	ChangeCursor(target);
}

/*
* 생성시 Particle하나 보유
* Tick 마다 Target이 Null이라면 Return 아니라면 Player를 Target방향으로 컨트롤러를 조금씩 회전
* 호출순서
* 1. Player Target Action : 
* 2. Start Target : TraceRadius만큼 충돌체검사 (Player제외, Pawn만 검사) 검사후 검출된것들 TraceTargets에 Add(중복제외)
* 3. Set Target : Player의 전방 벡터와 Player to 현재검출Pawn으로의 방향백터를 내적
*	내적값이 제일큰놈 즉 각도가 0에 가까운놈 다시말해 가장 Player의 전방에서 가까운놈을 Target으로 저장
* 4. Change Cursor : 현재 Target한테 Particle Attach
* 
*/