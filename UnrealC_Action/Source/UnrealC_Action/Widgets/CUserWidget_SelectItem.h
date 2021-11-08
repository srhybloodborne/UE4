#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SelectItem.generated.h"

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCUserWidget_SelectItem : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
		void Click();

	UFUNCTION(BlueprintCallable)
		void Hover();

	UFUNCTION(BlueprintCallable)
		void UnHover();

private:
	class UCUserWidget_Select* GetSelectWidget();
};
