#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Warp.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API ACDoAction_Warp : public ACDoAction
{
	GENERATED_BODY()

public:
	ACDoAction_Warp();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);
	bool UseCursorLocation();
private:
	class UDecalComponent* Decal;
	FVector Location;
};
