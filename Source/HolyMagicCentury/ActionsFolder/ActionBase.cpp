// Lordmatics Games December 2022


#include "HolyMagicCentury/ActionsFolder/ActionBase.h"

UActionBase::UActionBase() :	
	Super()
{
}

UActionBase::~UActionBase()
{
}

void UActionBase::InitialiseAction(UActionDataBase* ActionData)
{
}

bool UActionBase::IsActionBlacklisted(const FName& ActionName)
{
	//TODO: Implement this.
	return false;
}

UActionDataBase::UActionDataBase() :
	Super()
{

}

UActionDataBase::~UActionDataBase()
{

}

void UActionDataBase::InitialiseObject()
{
}
