#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{

}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}//for(data)
	}//for(i)

	//for (const FMontageData* data : Datas)
	//{
	//	if (!!data)
	//		CLog::Log(data->AnimMontage->GetPathName());
	//}
}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackstep()
{
	PlayAnimMontage(EStateType::Backstep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayHitted2()
{
	PlayAnimMontage(EStateType::Hitted2);
}

void UCMontagesComponent::PlayHitted3()
{
	PlayAnimMontage(EStateType::Hitted3);
}


void UCMontagesComponent::PlayGuard()
{
	PlayAnimMontage(EStateType::Guard);
}

void UCMontagesComponent::PlayAirHitted()
{
	PlayAnimMontage(EStateType::AirHitted);
}

void UCMontagesComponent::PlayChargeSkillHitted()
{
	PlayAnimMontage(EStateType::ChargeSkillHitted);
}

void UCMontagesComponent::PlayLastBaseAttackHitted()
{
	PlayAnimMontage(EStateType::LastBaseAttackHitted);
}

void UCMontagesComponent::PlayLastAirAttackHitted()
{
	PlayAnimMontage(EStateType::LastAirAttackHitted);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayExecution()
{
	PlayAnimMontage(EStateType::Execution);
}

void UCMontagesComponent::PlayExecution2()
{
	PlayAnimMontage(EStateType::Execution2);
}

void UCMontagesComponent::PlayExecution3()
{
	PlayAnimMontage(EStateType::Execution3);
}


void UCMontagesComponent::PlayExhausted()
{
	PlayAnimMontage(EStateType::Exhausted);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	}
}