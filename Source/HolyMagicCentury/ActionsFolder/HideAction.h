// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "HideAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName HideAction = FName(TEXT("HideAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UHideActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	UHideActionData();
	virtual ~UHideActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UHideAction : public UActionBase
{
	GENERATED_BODY()

public:

	UHideAction();
	virtual ~UHideAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& Hit);

	virtual const FName GetActionName() const { return ActionNames::HideAction; }

private:

	float PreviousVelocity;
	float JumpVelocity;
};
