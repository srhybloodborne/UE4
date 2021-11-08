#include "CBTTaskNode_UnEquipBow.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_UnEquipBow::UCBTTaskNode_UnEquipBow()
{
	NodeName = "UnEquipBow";
}

EBTNodeResult::Type UCBTTaskNode_UnEquipBow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	if(action->IsBowMode() == true && state->IsIdleMode())
		action->SetBowMode();


	return EBTNodeResult::InProgress; //매프레임마다 Idle모드인지 체크 아니라면 InProgress
}

void UCBTTaskNode_UnEquipBow::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);

	if (state->IsIdleMode() && action->IsUnarmedMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
