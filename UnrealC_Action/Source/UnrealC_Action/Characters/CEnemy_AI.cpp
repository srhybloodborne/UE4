#include "CEnemy_AI.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/CPatrolComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
	CHelpers::CreateActorComponent<UCPatrolComponent>(this, &Patrol, "Patrol");
}