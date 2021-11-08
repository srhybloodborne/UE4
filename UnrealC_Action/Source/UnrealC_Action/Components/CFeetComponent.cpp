#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	float leftDistance;
	FRotator leftRotation;
	Trace(LeftSocket, leftDistance, leftRotation);
	float rightDistance;
	FRotator rightRotation;

	Trace(RightSocket, rightDistance, rightRotation);

	float offset = FMath::Min(leftDistance, rightDistance);



	offset = offset > 0.0f ? 0.0f : offset;


	//FeetData Location
	{
		//�㸮�� ������ ���� ����������
		Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);
		//�㸮�� ������ŭ �߳��� ���÷���ߴ�
		Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, leftDistance - offset, DeltaTime, InterpSpeed);
		//��Ī�̶� ��������ߴ�
		Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);
	}



	//FeetData Rotation
	{

		Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
		Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
	}
}


void UCFeetComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.0f;

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z);

	float traceZ = start.Z - CapsuleHalfHeight - TraceDistance;
	FVector end = FVector(location.X, location.Y, traceZ);

	//DrawDebugLine(OwnerCharacter->GetWorld(), start, end, FColor::Green);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	//Line Single Trace
	FHitResult hitResult;
	//���̴� ä�� ��������
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignoreActors, DrawDebugType, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	CheckFalse(hitResult.IsValidBlockingHit());

	//ImpactPoint : Hitting ����,��Ȯ�ϰ�, 

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();

	//CLog::Print(hitResult.ImpactPoint, 0);
	//CLog::Print(hitResult.TraceEnd, 1);
	//CLog::Print(end, 2);

	OutDistance = OffsetDistance + length - TraceDistance;

	//TODO: IK2�ٽú���
	FVector normal = hitResult.ImpactNormal;
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);
}