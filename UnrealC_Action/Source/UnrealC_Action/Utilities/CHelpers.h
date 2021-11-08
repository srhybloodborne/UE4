#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

class UNREALC_ACTION_API CHelpers
{
public:
	/// <summary>
	///  �����ڴܿ����� ���డ��(ConstructorHelpers)
	///  T ���۷����� Ÿ�Կ� �°� ��ü ������ OutObject�� �Ҵ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="OutObject"></param>
	/// <param name="InPath"></param>
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Object;
	}
	/// <summary>
	/// T���۷���Ÿ�Կ� �´� StaticClass(UObject) ��ü������ T������ ĳ����
	/// obj�� �Ҵ��� �ٽ� OutObject�� �Ҵ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="OutObject"></param>
	/// <param name="InPath"></param>
	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		//MaterialInstanceConstant'/Game/Materials/M_White_inst.M_White_Inst'
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
	
		verifyf(!!obj, L"!!obj");

		*OutObject = obj;
	}

	//BP�� C�θ��� �ڽ��ε� �̷��� �ع����� BP�� ����� ���� �����
	//DefaultPawnClass = Cast<APawn>(ACPlayer::StaticClass());
	//Pawn���� �־���ߴ�
	//Ŭ���� Ÿ������ �θ����� �׻� �ڿ� _C�ٿ��ߵ�
	//ConstructorHelpers::FClassFinder<APawn> pawn(L"Blueprint'/Game/BP_CPlayer.BP_CPlayer_C'");
	//if (pawn.Succeeded())
	//	pawn.Class


	template<typename T>
	static void GetClass(TSubclassOf<T>* OutObject, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);

		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Class;
	}

	//From Scene
	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);
			return;
		}
		else
		{
			InActor->SetRootComponent((*InComponent));
		}
	}

	//From Actor
	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
};