#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Sword.generated.h"

/**
 *
 */
UCLASS()
class UNREALC_ACTION_API UCBTService_Sword : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Sword();


private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
