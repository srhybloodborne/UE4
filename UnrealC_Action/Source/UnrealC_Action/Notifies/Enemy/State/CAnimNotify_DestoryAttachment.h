// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_DestoryAttachment.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCAnimNotify_DestoryAttachment : public UAnimNotify
{
	GENERATED_BODY()
public:
		FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
