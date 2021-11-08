#include "CAIController.h"
#include "Global.h"
#include "CEnemy_AI.h"
#include "CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "BlackBoard");//부모에있음
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	
	//Sight = NewObject<UAISenseConfig_Sight>(); //동적할당 BeginPlay이후 에서만 사용가능 절대 UObject로 할당X
	//할당
	//Runtime = UObject
	//Editor = Create
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 800.0f;
	Sight->PeripheralVisionAngleDegrees = 90.0f;
	Sight->SetMaxAge(2.0f);

	//관계
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());//여러개의 감지 객체가 있을떄 우선순위로 지정
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(OwnerEnemy->GetTeamID());
	
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);//자기가 Possess된 enemy_ai의 BehaviorTree의 Blackboard사용
	Behavior->SetBlackboard(Blackboard);//BehaviorComponent의 Blackboar를 Enemy_Ai의 BehaviorTree의 BlackBoard로 연동


	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());//Enemy_AI의 BehaviorTree실행
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}


void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebug)
	{
		FVector center = OwnerEnemy->GetActorLocation();
		center.Z -= AdjustCircleHeight;
		//DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
		//DrawDebugCircle(GetWorld(), center, BehaivorRange, 300, FColor::Red, false, -1.0f, 0, 0, FVector::RightVector, FVector::ForwardVector);
	}
}
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;

	Perception->GetCurrentlyPerceivedActors(NULL, actors);//현제 감지된놈
	
	
	ACPlayer* player = NULL;
	ACEnemy_AI* enemy = NULL;

	for (AActor* actor : actors)
	{
		//CLog::Print(actor->GetName());

		player = Cast<ACPlayer>(actor);
		enemy = Cast<ACEnemy_AI>(actor);

		//CLog::Print(player->GetTeamID());
		//CLog::Print(OwnerEnemy->GetTeamID());

		//1. Enemy->player or player friend
		if (!!player && player->GetTeamID() != OwnerEnemy->GetTeamID())
		{
		
			Blackboard->SetValueAsObject("Player", player);//Blackboard에 Player에 player설정
			return;
		}
		if (!!enemy)
		{
			//CLog::Print(OwnerEnemy->GetTeamID());
			//CLog::Print(enemy->GetTeamID());
			
			if (!!enemy && enemy->GetTeamID() != OwnerEnemy->GetTeamID())
			{

				//CLog::Print("Check");
				Blackboard->SetValueAsObject("Player", enemy);//Blackboard에 Player에 player설정
				return;
			}
		}
	
	}
	if(player == NULL)
		Blackboard->SetValueAsObject("Player", player);//Blackboard에 Player에 player설정
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}
