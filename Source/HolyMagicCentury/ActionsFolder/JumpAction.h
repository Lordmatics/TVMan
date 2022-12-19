// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "JumpAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName JumpAction = FName(TEXT("JumpAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UJumpActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	UJumpActionData();
	virtual ~UJumpActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UJumpAction : public UActionBase
{
	GENERATED_BODY()

public:

	UJumpAction();
	virtual ~UJumpAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& Hit);

	virtual const FName GetActionName() const { return ActionNames::JumpAction; }

private:

};
