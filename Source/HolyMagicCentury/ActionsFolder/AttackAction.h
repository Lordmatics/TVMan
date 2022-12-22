// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "AttackAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName AttackAction = FName(TEXT("AttackAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UAttackActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	UAttackActionData();
	virtual ~UAttackActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UAttackAction : public UActionBase
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(UAttackAction, UAttackActionData);

	UAttackAction();
	virtual ~UAttackAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& HitResult) override;

	virtual const FName GetActionName() const { return ActionNames::AttackAction; }

	virtual void CancelAction() override;
	
private:

};
