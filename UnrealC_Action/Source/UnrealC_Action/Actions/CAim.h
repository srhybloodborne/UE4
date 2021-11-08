#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class UNREALC_ACTION_API UCAim : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

public:
	FORCEINLINE bool IsAvaliable() { return SpringArm != NULL && Camera != NULL; }
	FORCEINLINE bool InZoom() { return bInZoom; }


public:
	UCAim();
	void BeginPlay(class ACharacter* InCharacter);
	void Tick(float DeltaTime);

	void On();
	void Off();

private:
	UFUNCTION()
		void Zooming(float Value);

private:
	class ACharacter* OwnerCharacter;

	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	class ACHUD* Hud;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;

	bool bInZoom;
};
