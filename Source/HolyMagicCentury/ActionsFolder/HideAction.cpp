// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/HideAction.h"
#include "../CharactersFolder/BaseCharacter.h"
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "LeapAction.h"
#include "JumpAction.h"

UHideActionData::UHideActionData()
{

}

UHideActionData::~UHideActionData()
{

}

void UHideActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

UHideAction::UHideAction() :
	PreviousVelocity(150.0f),
	JumpVelocity(300.0f)
{
	Blacklist.Push(ActionNames::LeapAction);
	Blacklist.Push(ActionNames::JumpAction);	
}

UHideAction::~UHideAction()
{

}

void UHideAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	UHideActionData* ActionData = Cast<UHideActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetHiding(true);
		}	
		
		if (UCharacterMovementComponent* CharacterMovementComponent = BaseCharacter->GetCharacterMovement())
		{
			PreviousVelocity = CharacterMovementComponent->JumpZVelocity;
			CharacterMovementComponent->JumpZVelocity = JumpVelocity;
		}
	}
}

void UHideAction::OnActionCreated()
{

}

void UHideAction::OnActionProcess(const float DeltaTime)
{

}

void UHideAction::OnActionDestroyed()
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

	if (UCharacterMovementComponent* CharacterMovementComponent = BaseCharacter->GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = PreviousVelocity;
	}

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetHiding(false);
	}
}

void UHideAction::OnLanded(const FHitResult& Hit)
{

}
