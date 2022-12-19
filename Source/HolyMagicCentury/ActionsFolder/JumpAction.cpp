// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/JumpAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>

UJumpActionData::UJumpActionData()
{

}

UJumpActionData::~UJumpActionData()
{

}

void UJumpActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

UJumpAction::UJumpAction()
{

}

UJumpAction::~UJumpAction()
{

}

void UJumpAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	UJumpActionData* ActionData = Cast<UJumpActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
	
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			
		}		
	}
}

void UJumpAction::OnActionCreated()
{

}

void UJumpAction::OnActionProcess(const float DeltaTime)
{

}

void UJumpAction::OnActionDestroyed()
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
		
	}
}
