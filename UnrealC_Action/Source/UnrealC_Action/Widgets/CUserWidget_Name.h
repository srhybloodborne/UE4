#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Name.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCUserWidget_Name : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetNameText(const FString& InName);
	UFUNCTION(BlueprintImplementableEvent)
		void SetControllerText(const FString& InName);
};
