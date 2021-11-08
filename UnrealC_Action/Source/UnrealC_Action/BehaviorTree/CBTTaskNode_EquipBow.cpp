#include "CBTTaskNode_EquipBow.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_EquipBow::UCBTTaskNode_EquipBow()
{
	NodeName = "EquipBow";
}

EBTNodeResult::Type UCBTTaskNode_EquipBow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);

	if (action->IsUnarmedMode() == true)
		action->SetBowMode();

	return EBTNodeResult::Succeeded; //매프레임마다 Idle모드인지 체크 아니라면 InProgress
}

void UCBTTaskNode_EquipBow::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);

	if (action->IsBowMode())
	{
		//CLog::Print("check");
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
