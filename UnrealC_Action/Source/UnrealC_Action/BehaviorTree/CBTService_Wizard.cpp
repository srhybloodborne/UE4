#include "CBTService_Wizard.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"


UCBTService_Wizard::UCBTService_Wizard()
{
	NodeName = "Wizard";
}

void UCBTService_Wizard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//OwnerComp = BT_Melee, OwnerComp->GetOwenr() = ACAIController, contorller->GetPawn() = ACEnemy_AI
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	//DrawDebugCircle(ai->GetWorld(), ai->GetActorLocation(), controller->GetSightRadius(), 200, FColor::Green, false, 0.5f, 0, 0, FVector(1, 0, 0), FVector(0, 1, 0));
	//DrawDebugCircle(ai->GetWorld(), ai->GetActorLocation(), AvoidInRange, 200, FColor::Red, false, 0.5f, 0, 0, FVector(1, 0, 0), FVector(0, 1, 0));

	//Hit Check
	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();

		return;
	}

	//Perception Check
	ACPlayer* target = behavior->GetTargetPlayer();//Get Player From Balckboard
	if (target == NULL)
	{
		

		behavior->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::Default);
		return;
	}

	//Hit Radius Check
	float distance = ai->GetDistanceTo(target);
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetAvoidMode();

		return;
	}

	//Attack Radius Check
	controller->SetFocus(target);
	float approach = controller->GetSightRadius();
	if (distance < approach)
	{
		behavior->SetActionMode();

		return;
	}
}
