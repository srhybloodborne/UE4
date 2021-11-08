#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <UnrealC_Action/Actions/CAction.h>
#include "CActionData.generated.h"

UCLASS()
class UNREALC_ACTION_API UCActionData : public UDataAsset
{
	GENERATED_BODY()
public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction);
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> BaseComboClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> BaseComboDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> AirComboClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> AirComboDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> SecondComboClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> SecondComboDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> ChargeComboClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> ChargeComboDatas;


private:
//	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

};
