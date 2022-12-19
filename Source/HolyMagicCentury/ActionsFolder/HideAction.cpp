// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/HideAction.h"
#include "../CharactersFolder/BaseCharacter.h"
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "LeapAction.h"
#include "JumpAction.h"
#include <Components/CapsuleComponent.h>
#include "ActionManager.h"

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
	JumpVelocity(300.0f)
{
	//Blacklist.Push(ActionNames::LeapAction);
	Blacklist.Push(ActionNames::JumpAction);

	RegisterActionToManager(ActionNames::HideAction, UHideAction);
}

UHideAction::~UHideAction()
{

}

void UHideAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	Super::InitialiseAction(ActionDataBase);

	UHideActionData* ActionData = Cast<UHideActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{		
		const float DefaultGravityScale = BaseCharacter->GetDefaultGravity();
		const float DefaultJumpVelocity = BaseCharacter->GetDefaultJumpVelocity();
		const FName& LastKnownDefaultAction = BaseCharacter->GetLastKnownDefaultActionName();
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetHiding(true);
		}	
		
		if (UCharacterMovementComponent* CharacterMovementComponent = BaseCharacter->GetCharacterMovement())
		{
			CharacterMovementComponent->JumpZVelocity = JumpVelocity * DefaultGravityScale;	
		}
	}
}

void UHideAction::OnActionCreated()
{
	Super::OnActionCreated();
}

void UHideAction::OnActionProcess(const float DeltaTime)
{
	Super::OnActionProcess(DeltaTime);

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetHiding(true);
		}
	}
}

void UHideAction::OnActionDestroyed()
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

	const float DefaultJumpVelocity = BaseCharacter->GetDefaultJumpVelocity();
	if (UCharacterMovementComponent* CharacterMovementComponent = BaseCharacter->GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = DefaultJumpVelocity;
	}

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetHiding(false);
	}
}

void UHideAction::OnLanded(const FHitResult& Hit)
{
	Super::OnLanded(Hit);
}

void UHideAction::CancelAction()
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
