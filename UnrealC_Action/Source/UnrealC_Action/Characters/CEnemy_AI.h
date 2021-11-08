#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
	
public:
	ACEnemy_AI();
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCPatrolComponent* Patrol;
public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE int GetAttackCount() { return AttackCount; }
	FORCEINLINE void SetAttackCount(int InAttackCount) { AttackCount = InAttackCount; }

private:
	int AttackCount = 0;
};
