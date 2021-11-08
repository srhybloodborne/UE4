// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Select.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUserWidget_Select_Clicked);

USTRUCT(BlueprintType)
struct FUserWidget_Select_Data
{
	GENERATED_BODY()
public:
	class UCUserWidget_SelectItem* item;
	FUserWidget_Select_Clicked OnUserWidget_Select_Clicked;
};

/**
 * 
 */
UCLASS()
class UNREALC_ACTION_API UCUserWidget_Select : public UUserWidget
{
	GENERATED_BODY()
protected:
	void NativeConstruct() override;
public:
	FORCEINLINE FUserWidget_Select_Data& GetData(uint32 InIndex) { return Datas[InIndex]; }
public:
	void Clicked(FString InName);
	void Hovered(FString InName);
	void UnHovered(FString InName);

private:
	TArray<FUserWidget_Select_Data> Datas;
};
