// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/GroundSlamAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "LeapAction.h"
#include "HideAction.h"
#include "JumpAction.h"
#include "ActionManager.h"

UGroundSlamActionData::UGroundSlamActionData()
{

}

UGroundSlamActionData::~UGroundSlamActionData()
{

}

void UGroundSlamActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

UGroundSlamAction::UGroundSlamAction()
{
	Blacklist.Push(ActionNames::LeapAction);
	Blacklist.Push(ActionNames::HideAction);
	Blacklist.Push(ActionNames::JumpAction);

	RegisterActionToManager(ActionNames::GroundSlamAction, UGroundSlamAction);
}

UGroundSlamAction::~UGroundSlamAction()
{

}

void UGroundSlamAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	UGroundSlamActionData* ActionData = Cast<UGroundSlamActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
	
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetGroundSlamming(true);
		}		
	}
}

void UGroundSlamAction::OnActionCreated()
{

}

void UGroundSlamAction::OnActionProcess(const float DeltaTime)
{

}

void UGroundSlamAction::OnActionDestroyed()
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
		AnimInstance->SetGroundSlamming(false);
	}
}

void UGroundSlamAction::OnLanded(const FHitResult& HitResult)
{
	CancelAction();
}

void UGroundSlamAction::CancelAction()
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
		AnimInstance->SetGroundSlamming(false);
	}
}