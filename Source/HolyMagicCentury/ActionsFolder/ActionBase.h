// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.generated.h"

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UActionDataBase : public UObject
{
	GENERATED_BODY()

public:
	UActionDataBase();
	virtual ~UActionDataBase();

	virtual void InitialiseObject();

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UActionBase : public UObject
{
	GENERATED_BODY()

public:
	UActionBase();
	virtual ~UActionBase();

	virtual void InitialiseAction(UActionDataBase* ActionData);
	virtual bool IsActionBlacklisted(const FName& ActionName);

	virtual void OnActionCreated() PURE_VIRTUAL(UActionBase::OnActionCreated(), return;);
	virtual void OnActionProcess(const float DeltaTime) PURE_VIRTUAL(UActionBase::OnActionProcess(const float DeltaTime), return;);
	virtual void OnActionDestroyed() PURE_VIRTUAL(UActionBase::OnActionDestroyed(), return;);

	virtual const FName GetActionName() const PURE_VIRTUAL(UActionBase::OnActionDestroyed(), return TEXT("None"););
private:

protected:
};

