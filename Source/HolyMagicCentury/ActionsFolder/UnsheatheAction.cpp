// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/UnsheatheAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "ActionManager.h"
#include "GroundSlamAction.h"
#include "LeapAction.h"

UUnsheatheActionData::UUnsheatheActionData()
{

}

UUnsheatheActionData::~UUnsheatheActionData()
{

}

void UUnsheatheActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

UUnsheatheAction::UUnsheatheAction()
{
	Blacklist.Push(ActionNames::GroundSlamAction);
	Blacklist.Push(ActionNames::LeapAction);

	RegisterActionToManager(ActionNames::UnsheatheAction, UUnsheatheAction);
}

UUnsheatheAction::~UUnsheatheAction()
{

}

void UUnsheatheAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	UUnsheatheActionData* ActionData = Cast<UUnsheatheActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
	
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		FCharacterMontages& MontagePacket = BaseCharacter->GetMontagePacket();
		UAnimMontage* UnsheatheMontage = MontagePacket.UnsheatheMontage;
		if (!UnsheatheMontage)
		{
			return;
		}

		const float Duration = BaseCharacter->PlayAnimMontage(UnsheatheMontage, 1.0f, NAME_None);
		if (Duration >= 0.0f)
		{
			if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
			{				
				AnimInstance->SetUnsheatheAntenna(true);
			}
		}			
	}
}

void UUnsheatheAction::OnActionCreated()
{

}

void UUnsheatheAction::OnActionProcess(const float DeltaTime)
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
	UAnimMontage* UnsheatheMontage = MontagePacket.UnsheatheMontage;
	if (!UnsheatheMontage)
	{
		return;
	}

	const bool bIsPlaying = AnimInstance->Montage_IsPlaying(UnsheatheMontage);
	if (!bIsPlaying)
	{
		CancelAction();
	}
}

void UUnsheatheAction::OnActionDestroyed()
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
		AnimInstance->SetSheatheAntenna(false);
	}
}

void UUnsheatheAction::OnLanded(const FHitResult& HitResult)
{

}

void UUnsheatheAction::CancelAction()
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