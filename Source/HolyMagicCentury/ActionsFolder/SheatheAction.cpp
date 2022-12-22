// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/SheatheAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "ActionManager.h"
#include "GroundSlamAction.h"
#include "LeapAction.h"

USheatheActionData::USheatheActionData()
{

}

USheatheActionData::~USheatheActionData()
{

}

void USheatheActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

USheatheAction::USheatheAction()
{
	Blacklist.Push(ActionNames::GroundSlamAction);
	Blacklist.Push(ActionNames::LeapAction);

	RegisterActionToManager(ActionNames::SheatheAction, USheatheAction);
}

USheatheAction::~USheatheAction()
{

}

void USheatheAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	USheatheActionData* ActionData = Cast<USheatheActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
	
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		FCharacterMontages& MontagePacket = BaseCharacter->GetMontagePacket();
		UAnimMontage* SheatheMontage = MontagePacket.SheatheMontage;
		if (!SheatheMontage)
		{
			return;
		}

		const float Duration = BaseCharacter->PlayAnimMontage(SheatheMontage, 1.0f, NAME_None);
		if (Duration >= 0.0f)
		{
			if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
			{
				AnimInstance->SetSheatheAntenna(true);
			}
		}
	}
}

void USheatheAction::OnActionCreated()
{

}

void USheatheAction::OnActionProcess(const float DeltaTime)
{
	UObject* Owner = GetOuter();
	if (!Owner)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner);
	if (!BaseCharacter)
	{
		return;
	}

	UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	FCharacterMontages& MontagePacket = BaseCharacter->GetMontagePacket();
	UAnimMontage* SheatheMontage = MontagePacket.SheatheMontage;
	if (!SheatheMontage)
	{
		return;
	}

	const bool bIsPlaying = AnimInstance->Montage_IsPlaying(SheatheMontage);
	if (!bIsPlaying)
	{
		CancelAction();
	}
}

void USheatheAction::OnActionDestroyed()
{
	UObject* Owner = GetOuter();
	if (!Owner)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner);
	if (!BaseCharacter)
	{
		return;
	}

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetUnsheatheAntenna(false);
	}
}

void USheatheAction::OnLanded(const FHitResult& HitResult)
{

}

void USheatheAction::CancelAction()
{
	UObject* Owner = GetOuter();
	if (!Owner)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner);
	if (!BaseCharacter)
	{
		return;
	}

	BaseCharacter->EndAction();
}