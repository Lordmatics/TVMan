// Lordmatics Games December 2022

#include "HolyMagicCentury/ActionsFolder/DefaultAction.h"
#include "ActionManager.h"
#include "../CharactersFolder/BaseCharacter.h"
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"

UDefaultActionData::UDefaultActionData()
{

}

UDefaultActionData::~UDefaultActionData()
{

}

void UDefaultActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

UDefaultAction::UDefaultAction()
{	
	RegisterActionToManager(ActionNames::DefaultAction, UDefaultAction);
}

UDefaultAction::~UDefaultAction()
{

}

void UDefaultAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	UDefaultActionData* ActionData = Cast<UDefaultActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
}

void UDefaultAction::OnActionCreated()
{
	ResetAnimInstanceStates();
}

void UDefaultAction::OnActionProcess(const float DeltaTime)
{

}

void UDefaultAction::OnActionDestroyed()
{

}

void UDefaultAction::OnLanded(const FHitResult& Hit)
{

}

void UDefaultAction::ResetAnimInstanceStates()
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
		AnimInstance->ResetNonDefaults();
	}
}
