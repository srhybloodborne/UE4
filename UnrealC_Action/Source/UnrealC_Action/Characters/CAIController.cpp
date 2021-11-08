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

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "BlackBoard");//�θ�����
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	
	//Sight = NewObject<UAISenseConfig_Sight>(); //�����Ҵ� BeginPlay���� ������ ��밡�� ���� UObject�� �Ҵ�X
	//�Ҵ�
	//Runtime = UObject
	//Editor = Create
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 800.0f;
	Sight->PeripheralVisionAngleDegrees = 90.0f;
	Sight->SetMaxAge(2.0f);

	//����
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());//�������� ���� ��ü�� ������ �켱������ ����
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

	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);//�ڱⰡ Possess�� enemy_ai�� BehaviorTree�� Blackboard���
	Behavior->SetBlackboard(Blackboard);//BehaviorComponent�� Blackboar�� Enemy_Ai�� BehaviorTree�� BlackBoard�� ����


	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());//Enemy_AI�� BehaviorTree����
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

	Perception->GetCurrentlyPerceivedActors(NULL, actors);//���� �����ȳ�
	
	
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
		
			Blackboard->SetValueAsObject("Player", player);//Blackboard�� Player�� player����
			return;
		}
		if (!!enemy)
		{
			//CLog::Print(OwnerEnemy->GetTeamID());
			//CLog::Print(enemy->GetTeamID());
			
			if (!!enemy && enemy->GetTeamID() != OwnerEnemy->GetTeamID())
			{

				//CLog::Print("Check");
				Blackboard->SetValueAsObject("Player", enemy);//Blackboard�� Player�� player����
				return;
			}
		}
	
	}
	if(player == NULL)
		Blackboard->SetValueAsObject("Player", player);//Blackboard�� Player�� player����
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}
