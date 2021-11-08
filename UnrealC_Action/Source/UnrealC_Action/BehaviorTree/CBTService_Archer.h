#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Archer.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCBTService_Archer : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_Archer();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
