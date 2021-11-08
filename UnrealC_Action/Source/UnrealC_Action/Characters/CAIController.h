
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;
private:
	UPROPERTY(EditAnywhere)
		float BehaivorRange = 150.0f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;

	UPROPERTY(EditAnywhere)
		float AdjustCircleHeight = 50.0f;


public:
	FORCEINLINE float GetBehaviorRange() { return BehaivorRange; }
private:
//Delegate
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

public:
	float GetSightRadius();

private://Variable
	class ACEnemy_AI* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;
};
