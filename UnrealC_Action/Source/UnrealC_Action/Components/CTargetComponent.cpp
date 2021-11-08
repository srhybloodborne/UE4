#include "CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CStateComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//������ ��ƼŬ ������
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
	
	//ƽ���� Target�� NULL�� �ƴ϶�� Target�� state�� ������
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);

	//�Ÿ�üũ
	bool b = false;
	b |= state->IsDeadMode();
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;

	if (b == true)
	{
		EndTargeting();

		return;
	}

	//Player�κ��� Target�� ȸ���� ���ϱ�
	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	//DeltaTIme���� InteropSpeed���� ���� Player to Target�� ���� ���Ⱚ ȸ���� ����
	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InteropSpeed);
	OwnerCharacter->GetController()->SetControlRotation(rotator);//�ش簪���� ����ȸ��
}

//ȣ�� ���� 1.
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

//ȣ�� ���� 2.
void UCTargetComponent::SetTraceTargets()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	//�浹�� ���� TraceRadius��ŭ �׸��� TraceTargets�� ����(�ߺ�����)
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

//ȣ�� ����3.
void UCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* target = NULL;

	for (ACharacter* character : TraceTargets)
	{
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();//Player ��Ʈ�ѷ��� �������
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();//Player�κ��� ���� �浹Pawn���� ���ϴ� ���⺤��
		offset = offset.GetSafeNormal();
		float temp = direction | offset;//Player�� ���溤�Ϳ�, Player to ���� Pawn�� ������ ����

		if (temp < angle)//temp���� ����ū�� ã�°� �� ������ 0�� ������ ã�°� 
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
* ������ Particle�ϳ� ����
* Tick ���� Target�� Null�̶�� Return �ƴ϶�� Player�� Target�������� ��Ʈ�ѷ��� ���ݾ� ȸ��
* ȣ�����
* 1. Player Target Action : 
* 2. Start Target : TraceRadius��ŭ �浹ü�˻� (Player����, Pawn�� �˻�) �˻��� ����Ȱ͵� TraceTargets�� Add(�ߺ�����)
* 3. Set Target : Player�� ���� ���Ϳ� Player to �������Pawn������ ������͸� ����
*	�������� ����ū�� �� ������ 0�� ������ �ٽø��� ���� Player�� ���濡�� �������� Target���� ����
* 4. Change Cursor : ���� Target���� Particle Attach
* 
*/