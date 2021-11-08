

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_UnEquip.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCBTTaskNode_UnEquip : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_UnEquip();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
