// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "AttackAction.h"
#include "SpinningSlashAction.generated.h"

class ABaseCharacter;

/**
 *
 */

namespace ActionNames
{
	const FName SpinningSlashAction = FName(TEXT("SpinningSlashAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API USpinningSlashActionData : public UAttackActionData
{
	GENERATED_BODY()

public:
	USpinningSlashActionData();
	virtual ~USpinningSlashActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API USpinningSlashAction : public UAttackAction
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(USpinningSlashAction, USpinningSlashActionData);

	USpinningSlashAction();
	virtual ~USpinningSlashAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& HitResult) override;

	virtual const FName GetActionName() const { return ActionNames::SpinningSlashAction; }

	virtual void CancelAction() override;

private:

	bool CheckForCollision(ABaseCharacter& BaseCharacter);

private:

	float Timer;
	float RaycastInterval;
	int Counter;
	int CollisionChecksUntilCompletion;

};