

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Katana.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCBTService_Katana : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Katana();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
