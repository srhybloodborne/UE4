#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CBehaviorComponent.h"

ACDoAction_Warp::ACDoAction_Warp()
{

}

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* actor : OwnerCharacter->Children)
	{
		//AttachmentÀÇ¹Ì
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Warp"))
		{
			Decal = CHelpers::GetComponent<UDecalComponent>(actor);

			break;
		}
	}



}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckFalse(*bEquipped);

	if (UseCursorLocation() == false)
	{
		Decal->SetVisibility(false);

		return;
	}

	FVector location;
	FRotator rotator;


	if (GetCursorLocationAndRotation(location, rotator) == true)
	{
		Decal->SetVisibility(true);
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotator);
	}
	else
		Decal->SetVisibility(false);


	
}

void ACDoAction_Warp::DoAction()
{
	CheckFalse(*bEquipped);
	CheckFalse(State->IsIdleMode());

	if (UseCursorLocation()==false)
	{
		FRotator rotator;
		CheckFalse(GetCursorLocationAndRotation(Location, rotator));
	}
	else
	{
		AAIController* controller = OwnerCharacter->GetController<AAIController>();
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

		Location = behavior->GetWarpLocation();
		CLog::Print(Location);
		Decal->SetVisibility(false);
	}

	State->SetActionMode();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Warp::Begin_DoAction()
{
	FTransform transform = Datas[0].EffectTransform;
	UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, OwnerCharacter->GetMesh(), "",transform.GetLocation(),FRotator(transform.GetRotation()),transform.GetScale3D());
}

void ACDoAction_Warp::End_DoAction()
{
	OwnerCharacter->SetActorLocation(Location);
	Location = FVector::ZeroVector;

	State->SetIdleMode();
	Status->CanMove();
}

bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1); //World Static
	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(objects, false, hitResult)) //TraceComples = TriangleSeartch
	{
		OutLocation = hitResult.Location;
		OutRotation = hitResult.ImpactNormal.Rotation();

		return true;
	}

	return false;
}

bool ACDoAction_Warp::UseCursorLocation()
{
	return OwnerCharacter->GetController<AAIController>() == NULL;
}