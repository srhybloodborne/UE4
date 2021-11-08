#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

class UNREALC_ACTION_API CHelpers
{
public:
	/// <summary>
	///  생성자단에서만 실행가능(ConstructorHelpers)
	///  T 레퍼런스의 타입에 맞게 객체 생성후 OutObject에 할당
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
	/// T레퍼런스타입에 맞는 StaticClass(UObject) 객체생성후 T형으로 캐스팅
	/// obj에 할당을 다시 OutObject에 할당
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

	//BP는 C로만든 자식인데 이렇게 해버리면 BP의 기능을 전부 상실함
	//DefaultPawnClass = Cast<APawn>(ACPlayer::StaticClass());
	//Pawn으로 넣어줘야댐
	//클래스 타입으로 부를떄는 항상 뒤에 _C붙여야됨
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