#include "CBTService_Archer.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"
#include "Components/CActionComponent.h"


UCBTService_Archer::UCBTService_Archer()
{
	NodeName = "Arhcer";
}

void UCBTService_Archer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
		controller->ClearFocus(EAIFocusPriority::Default);
		return;
	}

	//Å¸°ÙÀÖÀ»‹š ¾ð¾ÏµåÀÏ‹š ÀåÂø
	if (action->IsUnarmedMode() == true)
	{
		behavior->SetEquipMode();
		return;
	}

	controller->SetFocus(target);
	float distance = ai->GetDistanceTo(target);
	float approach = controller->GetSightRadius();
	//Hit Radius Check
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetAvoidMode();

		return;
	}

	//Attack Radius Check
	if (distance < approach)
	{
		behavior->SetActionMode();

		return;
	}
}
