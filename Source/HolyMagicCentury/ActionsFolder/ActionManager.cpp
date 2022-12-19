// Lordmatics Games December 2022


#include "HolyMagicCentury/ActionsFolder/ActionManager.h"
#include "ActionBase.h"
#include "LeapAction.h"
#include "DefaultAction.h"
#include "HideAction.h"
#include "JumpAction.h"
#include "GroundSlamAction.h"

TMap<FName, UActionManager::CreateActionFuncPtr> UActionManager::MapActionTypes;

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

	// Get Static Create Function mapped to action name.
	auto FuncPtr = MapActionTypes[ActionName];
	if (UActionBase* CreatedAction = FuncPtr(Data, OuterObject))
	{
		SetCurrentAction(CreatedAction);
		return true;
	}

	// If this is hit, you forgot to add critical macros to your new action.
	checkNoEntry();
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
