// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/AttackAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "ActionManager.h"

UAttackActionData::UAttackActionData()
{

}

UAttackActionData::~UAttackActionData()
{

}

void UAttackActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

UAttackAction::UAttackAction()
{
	RegisterActionToManager(ActionNames::AttackAction, UAttackAction);
}

UAttackAction::~UAttackAction()
{

}

void UAttackAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	UAttackActionData* ActionData = Cast<UAttackActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
	
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetAttacking(true);
		}		
	}
}

void UAttackAction::OnActionCreated()
{

}

void UAttackAction::OnActionProcess(const float DeltaTime)
{

}

void UAttackAction::OnActionDestroyed()
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
		AnimInstance->SetAttacking(false);
	}
}

void UAttackAction::OnLanded(const FHitResult& HitResult)
{

}

void UAttackAction::CancelAction()
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