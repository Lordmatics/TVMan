// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "AttackAction.h"
#include "LungeAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName LungeAction = FName(TEXT("LungeAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API ULungeActionData : public UAttackActionData
{
	GENERATED_BODY()

public:
	ULungeActionData();
	virtual ~ULungeActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API ULungeAction : public UAttackAction
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(ULungeAction, ULungeActionData);

	ULungeAction();
	virtual ~ULungeAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& HitResult) override;

	virtual const FName GetActionName() const { return ActionNames::LungeAction; }

	virtual void CancelAction() override;
	
private:

};