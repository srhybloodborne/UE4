

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_EquipBow.generated.h"

/**
 *
 */
UCLASS()
class UNREALC_ACTION_API UCBTTaskNode_EquipBow : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_EquipBow();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
