// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "AttackAction.h"
#include "SlashAction.generated.h"

class ABaseCharacter;

/**
 *
 */

namespace ActionNames
{
	const FName SlashAction = FName(TEXT("SlashAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API USlashActionData : public UAttackActionData
{
	GENERATED_BODY()

public:
	USlashActionData();
	virtual ~USlashActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API USlashAction : public UAttackAction
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(USlashAction, USlashActionData);

	USlashAction();
	virtual ~USlashAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& HitResult) override;

	virtual const FName GetActionName() const { return ActionNames::SlashAction; }

	virtual void CancelAction() override;

private:

	bool CheckForCollision(ABaseCharacter& BaseCharacter);

private:

	float Timer;
	float RaycastInterval;

};