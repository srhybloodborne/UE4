#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndSecondComboCombo.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCAnimNotify_EndSecondComboCombo : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	
};
