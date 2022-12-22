// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "UnsheatheAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName UnsheatheAction = FName(TEXT("UnsheatheAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UUnsheatheActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	UUnsheatheActionData();
	virtual ~UUnsheatheActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UUnsheatheAction : public UActionBase
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(UUnsheatheAction, UUnsheatheActionData);

	UUnsheatheAction();
	virtual ~UUnsheatheAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& HitResult) override;

	virtual const FName GetActionName() const { return ActionNames::UnsheatheAction; }

	virtual void CancelAction() override;
	
private:

};
