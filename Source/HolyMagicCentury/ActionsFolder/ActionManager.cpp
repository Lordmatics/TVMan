// Lordmatics Games December 2022


#include "HolyMagicCentury/ActionsFolder/ActionManager.h"
#include "ActionBase.h"
#include "LeapAction.h"
#include "DefaultAction.h"
#include "HideAction.h"
#include "JumpAction.h"

UActionManager::UActionManager() :
	CurrentAction(nullptr)
{
}

UActionManager ::~UActionManager()
{
}

void UActionManager::InitialiseManager(UObject* Owner)
{
	
}

bool UActionManager::RequestAction(const FName& ActionName, UActionDataBase* Data)
{
	UActionBase* Current = CurrentAction.Get();
	if (!Current)
	{
		return CreateActionFromName(ActionName, Data);		
	}

	const bool bDenyTransition = Current->IsActionBlacklisted(ActionName);
	if (bDenyTransition)
	{
		return false;
	}

	return CreateActionFromName(ActionName, Data);	
}

bool UActionManager::Process(const float DeltaTime)
{
	UActionBase* Current = CurrentAction.Get();
	if (!Current)
	{
		return false;
	}

	Current->OnActionProcess(DeltaTime);
	return true;
}

void UActionManager::EndCurrentAction()
{
	SetCurrentAction(nullptr);
}

const FString UActionManager::GetCurrentActionName() const
{
	if (UActionBase* Current = CurrentAction.Get())
	{
		return Current->GetActionName().ToString();
	}

	static FString local(TEXT("None"));
	return local;
}

const bool UActionManager::IsCurrentAction(const FName& ActionName)
{
	if (UActionBase* Current = CurrentAction.Get())
	{
		return Current->GetActionName().IsEqual(ActionName);
	}
	return false;
}

void UActionManager::OnLanded(const FHitResult& Hit)
{
	if (UActionBase* Current = CurrentAction.Get())
	{
		return Current->OnLanded(Hit);
	}
}

bool UActionManager::CreateActionFromName(const FName& ActionName, UActionDataBase* Data)
{
	UObject* OuterObject = GetOuter();
	if (!OuterObject)
	{
		return false;
	}

	Data->InitialiseObject();

	if (ActionName == ActionNames::DefaultAction)
	{
		UDefaultActionData* DefaultData = Cast<UDefaultActionData>(Data);
		UDefaultAction* DefaultAction = NewObject<UDefaultAction>(OuterObject);
		DefaultAction->InitialiseAction(DefaultData);
		SetCurrentAction(DefaultAction);
		return true;
	}
	else if (ActionName == ActionNames::LeapAction)
	{
		ULeapActionData* LeapData = Cast<ULeapActionData>(Data);
		ULeapAction* LeapAction = NewObject<ULeapAction>(OuterObject);
		LeapAction->InitialiseAction(LeapData);
		SetCurrentAction(LeapAction);		
		return true;
	}
	else if (ActionName == ActionNames::HideAction)
	{
		UHideActionData* ActionData = Cast<UHideActionData>(Data);
		UHideAction* NewAction = NewObject<UHideAction>(OuterObject);
		NewAction->InitialiseAction(ActionData);
		SetCurrentAction(NewAction);
		return true;
	}
	else if (ActionName == ActionNames::JumpAction)
	{
		UJumpActionData* ActionData = Cast<UJumpActionData>(Data);
		UJumpAction* NewAction = NewObject<UJumpAction>(OuterObject);
		NewAction->InitialiseAction(ActionData);
		SetCurrentAction(NewAction);
		return true;
	}
	return false;
}

void UActionManager::SetCurrentAction(UActionBase* NewAction)
{
	if (UActionBase* Current = CurrentAction.Get())
	{
		Current->OnActionDestroyed();
	}

	CurrentAction = NewAction;
	if (NewAction)
	{
		NewAction->OnActionCreated();
	}	
}
