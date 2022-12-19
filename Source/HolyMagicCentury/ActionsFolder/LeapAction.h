// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "HolyMagicCentury/ActionsFolder/ActionBase.h"
#include "LeapAction.generated.h"

/**
 * 
 */

namespace ActionNames
{
	const FName LeapAction = FName(TEXT("LeapAction"));
}

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API ULeapActionData : public UActionDataBase
{
	GENERATED_BODY()

public:
	ULeapActionData();
	virtual ~ULeapActionData();

	virtual void InitialiseObject() override;

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API ULeapAction : public UActionBase
{
	GENERATED_BODY()

public:

	RegisterCreateFunction(ULeapAction, ULeapActionData);

	ULeapAction();
	virtual ~ULeapAction();	

	virtual void InitialiseAction(UActionDataBase* ActionData) override;

	virtual void OnActionCreated() override;
	virtual void OnActionProcess(const float DeltaTime) override;
	virtual void OnActionDestroyed() override;

	virtual void OnLanded(const FHitResult& Hit);

	virtual const FName GetActionName() const { return ActionNames::LeapAction; }

	virtual void CancelAction() override;

	UFUNCTION()
	void OnRaycastHit();

private:

	float Timer;
	float ProcessDelayTime;
	float Expiration;
};