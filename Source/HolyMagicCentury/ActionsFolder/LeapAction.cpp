// Lordmatics Games December 2022


#include "HolyMagicCentury/ActionsFolder/LeapAction.h"
#include "../CharactersFolder/BaseCharacter.h"
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#include "HideAction.h"
#include "JumpAction.h"
#include "ActionManager.h"
#include "../AnimationFolder/Notifies/AnimNotify_RaycastDown.h"

ULeapAction::ULeapAction() :
	Super(),
	Timer(0.0f),
	ProcessDelayTime(0.8f),
	Expiration(7.5f)
{
	Blacklist.Push(ActionNames::JumpAction);
	Blacklist.Push(ActionNames::HideAction);
	
	RegisterActionToManager(ActionNames::LeapAction, ULeapAction);
}

ULeapAction::~ULeapAction()
{

}

void ULeapAction::InitialiseAction(UActionDataBase* ActionData)
{
	ULeapActionData* LeapData = Cast<ULeapActionData>(ActionData);
	if (!LeapData)
	{
		return;
	}

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetLeaping(true);
		}

		if (BaseCharacter->GetLastKnownDefaultActionName() == ActionNames::HideAction)
		{

		}
		else
		{
			// Only TV Man needs to fly when leaping.
			BaseCharacter->SetFlying(true);
		}		
	}
}

void ULeapAction::OnActionCreated()
{
	Timer = 0.0f;

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		const FName& LastKnownDefaultActionNme = BaseCharacter->GetLastKnownDefaultActionName();
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			if (LastKnownDefaultActionNme == ActionNames::HideAction)
			{
				AnimInstance->SetHiding(true);
			}
		}

		if (LastKnownDefaultActionNme == ActionNames::HideAction)
		{
			
		}
		else
		{
			// Only TV Man needs to fly when leaping.
			BaseCharacter->SetFlying(true);
		}		
	}

	UAnimNotify_RaycastDown::OnCollisionDetected.AddDynamic(this, &ULeapAction::OnRaycastHit);
}

void ULeapAction::OnActionProcess(const float DeltaTime)
{
	Timer += DeltaTime;
	if (Timer <= ProcessDelayTime)
	{
		return;
	}

	if (Timer >= Expiration)
	{
		CancelAction();
		return;
	}

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetLeaping(true);
		}
	}
}

void ULeapAction::OnActionDestroyed()
{
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetLeaping(false);
		}

		BaseCharacter->SetFlying(false);
	}

	UAnimNotify_RaycastDown::OnCollisionDetected.RemoveDynamic(this, &ULeapAction::OnRaycastHit);
}

void ULeapAction::OnLanded(const FHitResult& Hit)
{
	// Determine when the leap should finish. Do a raycast down
	CancelAction();
}

void ULeapAction::CancelAction()
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

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetLeaping(false);
	}
}

void ULeapAction::OnRaycastHit()
{
	CancelAction();
}

ULeapActionData::ULeapActionData() :
	Super()
{

}

ULeapActionData::~ULeapActionData()
{

}

void ULeapActionData::InitialiseObject()
{
	Super::InitialiseObject();
}
