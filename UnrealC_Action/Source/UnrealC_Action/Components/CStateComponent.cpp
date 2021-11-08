#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetBackstepMode()
{
	ChangeType(EStateType::Backstep);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetChargeMode()
{
	ChangeType(EStateType::Charge);
}

void UCStateComponent::SetGuaradMode()
{
	ChangeType(EStateType::Guard);
}

void UCStateComponent::SetChargeSkillHitted()
{
	ChangeType(EStateType::ChargeSkillHitted);
}

void UCStateComponent::SetAirHittedMode()
{
	ChangeType(EStateType::AirHitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetExecutionMode()
{
	ChangeType(EStateType::Execution);
}

void UCStateComponent::SetExhaustedMode()
{
	ChangeType(EStateType::Exhausted);
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prevType = Type;
	Type = InNewType;
	
	if (OnStateTypeChanged.IsBound())
	{
		OnStateTypeChanged.Broadcast(prevType, InNewType);
	}
}
