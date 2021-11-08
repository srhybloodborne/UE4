
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Wizard.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCBTService_Wizard : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Wizard();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
