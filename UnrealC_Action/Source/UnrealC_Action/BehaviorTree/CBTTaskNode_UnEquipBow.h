

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_UnEquipBow.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCBTTaskNode_UnEquipBow : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_UnEquipBow();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
