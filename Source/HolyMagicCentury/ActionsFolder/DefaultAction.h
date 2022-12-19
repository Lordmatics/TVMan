// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "DefaultAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName DefaultAction = FName(TEXT("DefaultAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UDefaultActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	UDefaultActionData();
	virtual ~UDefaultActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UDefaultAction : public UActionBase
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(UDefaultAction, UDefaultActionData);

	UDefaultAction();
	virtual ~UDefaultAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& Hit);

	virtual const FName GetActionName() const { return ActionNames::DefaultAction; }

private:

};