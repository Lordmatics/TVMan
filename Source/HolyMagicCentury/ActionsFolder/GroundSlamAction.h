// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "GroundSlamAction.generated.h"

/**
 *
 */

namespace ActionNames
{
	const FName GroundSlamAction = FName(TEXT("GroundSlamAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UGroundSlamActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	UGroundSlamActionData();
	virtual ~UGroundSlamActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UGroundSlamAction : public UActionBase
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(UGroundSlamAction, UGroundSlamActionData);

	UGroundSlamAction();
	virtual ~UGroundSlamAction();

	virtual void InitialiseAction(UActionDataBase* ActionDataBase) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& HitResult) override;

	virtual const FName GetActionName() const { return ActionNames::GroundSlamAction; }

	virtual void CancelAction() override;
	
private:

};
