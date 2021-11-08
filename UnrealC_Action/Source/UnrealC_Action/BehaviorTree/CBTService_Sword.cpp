#include "CBTService_Sword.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"
#include "Components/CActionComponent.h"

UCBTService_Sword::UCBTService_Sword()
{
	NodeName = "Sword";
}

void UCBTService_Sword::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//OwnerComp = BT_Melee, OwnerComp->GetOwenr() = ACAIController, contorller->GetPawn() = ACEnemy_AI
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCPatrolComponent* Patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);


	//Hit Check
	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();

		return;
	}

	//Perception Check
	
	//zACPlayer* target = behavior->GetTargetPlayer();//Get Player From Balckboard
	//Perception Check
	AActor* target = NULL;
	ACPlayer* player = behavior->GetTargetPlayer();
	ACEnemy_AI* enemy = behavior->GetTargetEnemy();



	if (!!player)
	{
		target = player;
	}
	if (!!enemy)
	{
		target = enemy;
	}

	float distance = ai->GetDistanceTo(target);
	float approach = controller->GetSightRadius();

	if (target == NULL)
	{
		if (action->IsUnarmedMode() == false)
		{
			behavior->SetUnEquipMode();

			return;
		}

		if (action->IsUnarmedMode() == true)
		{
			if (Patrol != NULL && Patrol->IsValid())
			{
				behavior->SetPatrolMode();

				return;
			}
		}

		behavior->SetWaitMode();
		return;
	}

	if (action->IsUnarmedMode() == true)
	{
		behavior->SetEquipMode();
		return;
	}

	float dotProductToTarget = target->GetDotProductTo(ai);
	//뒤통수
	if (dotProductToTarget <= -0.25f)
	{
		//Hit Radius Check
		if (distance < controller->GetBehaviorRange())
		{
			behavior->SetActionMode();

			return;
		}
		else//때리러 간드앗
		{
			if (state->IsIdleMode() == false)
			{
				return;
			}

			behavior->SetApproachMode();

			return;
		}

	}

	//정면
	//Hit Radius Check
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetActionMode();

		return;
	}



	//Approach Radius Check
	if (distance < approach)
	{


		if (state->IsIdleMode() == false)
		{
			return;
		}

		behavior->SetAvoidMode();

		return;

	}
}
