#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"
#include "Components/CActionComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	ACPlayer* player = behavior->GetTargetPlayer();
	ACEnemy_AI* enemy = behavior->GetTargetEnemy();
	
	if (!!player)
	{

		ACPlayer* target = player;

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



		//Hit Radius Check
		float distance = ai->GetDistanceTo(target);
		if (distance < controller->GetBehaviorRange())
		{
			behavior->SetActionMode();

			return;
		}

		//Approach Radius Check
		float approach = controller->GetSightRadius();
		if (distance < approach)
		{
			if (state->IsIdleMode() == false)
			{
				return;
			}

			behavior->SetApproachMode();

			return;
		}
	}
	if (!!enemy)
	{
		ACEnemy_AI* target = enemy;

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



		//Hit Radius Check
		float distance = ai->GetDistanceTo(target);
		if (distance < controller->GetBehaviorRange())
		{
			behavior->SetActionMode();

			return;
		}

		//Approach Radius Check
		float approach = controller->GetSightRadius();
		if (distance < approach)
		{
			if (state->IsIdleMode() == false)
			{
				return;
			}

			behavior->SetApproachMode();

			return;
		}
	}
		

	
}
