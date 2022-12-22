// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/LungeAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "ActionManager.h"
#include "../CharactersFolder/CharacterMontages.h"

ULungeActionData::ULungeActionData()
{

}

ULungeActionData::~ULungeActionData()
{

}

void ULungeActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

ULungeAction::ULungeAction()
{
	RegisterActionToManager(ActionNames::LungeAction, ULungeAction);
}

ULungeAction::~ULungeAction()
{

}

void ULungeAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	Super::InitialiseAction(ActionDataBase);

	ULungeActionData* ActionData = Cast<ULungeActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
	
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		FCharacterMontages& MontagePacket = BaseCharacter->GetMontagePacket();
		UAnimMontage* Montage = MontagePacket.LungeMontage;
		if (!Montage)
		{
			return;
		}

		const float Duration = BaseCharacter->PlayAnimMontage(Montage, 1.0f, NAME_None);
		if (Duration >= 0.0f)
		{
			if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
			{
				AnimInstance->SetLunging(true);
			}
		}
	}
}

void ULungeAction::OnActionCreated()
{
	Super::OnActionCreated();
}

void ULungeAction::OnActionProcess(const float DeltaTime)
{
	Super::OnActionProcess(DeltaTime);

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
	UAnimMontage* Montage = MontagePacket.LungeMontage;
	if (!Montage)
	{
		return;
	}

	const bool bIsPlaying = AnimInstance->Montage_IsPlaying(Montage);
	if (!bIsPlaying)
	{
		CancelAction();
	}
}

void ULungeAction::OnActionDestroyed()
{
	Super::OnActionDestroyed();

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
		AnimInstance->SetLunging(false);
	}
}

void ULungeAction::OnLanded(const FHitResult& HitResult)
{
	Super::OnLanded(HitResult);
}

void ULungeAction::CancelAction()
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