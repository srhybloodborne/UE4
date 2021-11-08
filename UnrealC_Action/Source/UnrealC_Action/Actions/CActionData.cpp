#include "CActionData.h"
#include "Global.h"
#include "CAction.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"


void UCActionData::BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction)
{
	FTransform transform;

	ACAttachment* Attachment = NULL;

	if (!!AttachmentClass)
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		//Attachment->SetActorLabel(GetLabelName(InOwnerCharacter, "Attachment"));
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}

	ACEquipment* Equipment = NULL;

	if (!!EquipmentClass)
	{
		//SpawnActor-> 생성후 바로 등장 그래서 BeginPlay바로실행
		//SpawnActorDeferred -> 생성후 확정후 등장 
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		//Equipment->SetActorLabel(GetLabelName(InOwnerCharacter,"Equipment"));
		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		UGameplayStatics::FinishSpawningActor(Equipment, transform);
		
		if (!!Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnEquip);
		}
	}

	ACDoAction* BaseCombo = NULL;

	if (!!BaseComboClass)
	{
		//SpawnActor-> 생성후 바로 등장 그래서 BeginPlay바로실행
		//SpawnActorDeferred -> 생성후 확정후 등장 
		BaseCombo = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(BaseComboClass, transform, InOwnerCharacter);
		BaseCombo->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		//BaseCombo->SetActorLabel(GetLabelName(InOwnerCharacter, "BaseCombo"));
		BaseCombo->SetDatas(BaseComboDatas);
		UGameplayStatics::FinishSpawningActor(BaseCombo, transform);

		if (!!Equipment)
		{
			BaseCombo->SetEquipped(Equipment->GetEquipped());
		}

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(BaseCombo, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(BaseCombo, &ACDoAction::OnAttachmentEndOverlap);
		}
	}

	ACDoAction* AirCombo = NULL;

	if (!!AirComboClass)
	{
		//SpawnActor-> 생성후 바로 등장 그래서 BeginPlay바로실행
		//SpawnActorDeferred -> 생성후 확정후 등장 
		AirCombo = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(AirComboClass, transform, InOwnerCharacter);
		AirCombo->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		//AirCombo->SetActorLabel(GetLabelName(InOwnerCharacter, "AirCombo"));
		AirCombo->SetDatas(AirComboDatas);
		UGameplayStatics::FinishSpawningActor(AirCombo, transform);

		if (!!Equipment)
		{
			AirCombo->SetEquipped(Equipment->GetEquipped());
		}
		
		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(AirCombo, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(AirCombo, &ACDoAction::OnAttachmentEndOverlap);
		}
	}

	ACDoAction* SecondCombo = NULL;

	if (!!SecondComboClass)
	{
		//SpawnActor-> 생성후 바로 등장 그래서 BeginPlay바로실행
		//SpawnActorDeferred -> 생성후 확정후 등장 
		SecondCombo = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(SecondComboClass, transform, InOwnerCharacter);
		SecondCombo->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		//SecondCombo->SetActorLabel(GetLabelName(InOwnerCharacter, "SecondCombo"));
		SecondCombo->SetDatas(SecondComboDatas);
		UGameplayStatics::FinishSpawningActor(SecondCombo, transform);

		if (!!Equipment)
		{
			SecondCombo->SetEquipped(Equipment->GetEquipped());
		}

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(SecondCombo, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(SecondCombo, &ACDoAction::OnAttachmentEndOverlap);
		}
	}

	ACDoAction* ChargeCombo = NULL;

	if (!!ChargeComboClass)
	{
		//SpawnActor-> 생성후 바로 등장 그래서 BeginPlay바로실행
		//SpawnActorDeferred -> 생성후 확정후 등장 
		ChargeCombo = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(ChargeComboClass, transform, InOwnerCharacter);
		ChargeCombo->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		//ChargeCombo->SetActorLabel(GetLabelName(InOwnerCharacter, "ChargeCombo"));
		ChargeCombo->SetDatas(ChargeComboDatas);
		UGameplayStatics::FinishSpawningActor(ChargeCombo, transform);

		if (!!Equipment)
		{
			ChargeCombo->SetEquipped(Equipment->GetEquipped());
		}


		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(ChargeCombo, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(ChargeCombo, &ACDoAction::OnAttachmentEndOverlap);
		}
	}
	//TODO : Friend class가 뭐임
	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = Attachment;
	(*OutAction)->Equipment = Equipment;
	(*OutAction)->BaseCombo = BaseCombo;
	(*OutAction)->AirCombo = AirCombo;
	(*OutAction)->SecondCombo = SecondCombo;
	(*OutAction)->ChargeCombo = ChargeCombo;
	(*OutAction)->EquipmentColor = EquipmentColor;
}

//FString UCActionData::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
//{
//	FString name;
//	name.Append(InOwnerCharacter->GetActorLabel());
//	name.Append("_");
//	name.Append(InName);
//	name.Append("_");
//	name.Append(GetName().Replace(L"DA_", L""));
//
//	return name;
//}