// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "SheatheAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName SheatheAction = FName(TEXT("SheatheAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API USheatheActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	USheatheActionData();
	virtual ~USheatheActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API USheatheAction : public UActionBase
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(USheatheAction, USheatheActionData);

	USheatheAction();
	virtual ~USheatheAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& HitResult) override;

	virtual const FName GetActionName() const { return ActionNames::SheatheAction; }

	virtual void CancelAction() override;
	
private:

};
