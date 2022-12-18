// Lordmatics Games December 2022

#include "HolyMagicCentury/ActionsFolder/DefaultAction.h"

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
