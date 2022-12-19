// Lordmatics Games December 2022

#include "HolyMagicCentury/ActionsFolder/DefaultAction.h"
#include "ActionManager.h"

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
