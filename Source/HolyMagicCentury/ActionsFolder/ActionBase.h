// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.generated.h"

#define RegisterActionToManager(ActionName, ClassName) \
	TTuple<FName, UActionBase* (*)(UActionDataBase* DataPtr, UObject* OuterObject)> Tuple(ActionName, &ClassName::Create); \
	UActionManager::MapActionTypes.Add(Tuple); \

#define RegisterCreateFunction(ClassName, DataClassName) \
	static UActionBase* ClassName::Create(UActionDataBase* DataPtr, UObject* OuterObject)\
	{\
		DataClassName* ActionData = Cast<DataClassName>(DataPtr);\
		ClassName* NewAction = NewObject<ClassName>(OuterObject);\
		NewAction->InitialiseAction(ActionData);\
		return NewAction;\
	}\
	
UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UActionDataBase : public UObject
{
	GENERATED_BODY()

public:
	UActionDataBase();
	virtual ~UActionDataBase();

	virtual void InitialiseObject();

private:

};

UCLASS(EditInlineNew)
class HOLYMAGICCENTURY_API UActionBase : public UObject
{
	GENERATED_BODY()

public:
	UActionBase();
	virtual ~UActionBase();

	virtual void InitialiseAction(UActionDataBase* ActionData);
	virtual bool IsActionBlacklisted(const FName& ActionName);

	virtual void OnActionCreated() PURE_VIRTUAL(UActionBase::OnActionCreated(), return;);
	virtual void OnActionProcess(const float DeltaTime) PURE_VIRTUAL(UActionBase::OnActionProcess(const float DeltaTime), return;);
	virtual void OnActionDestroyed() PURE_VIRTUAL(UActionBase::OnActionDestroyed(), return;);

	virtual void OnLanded(const FHitResult& Hit) {}

	virtual const FName GetActionName() const PURE_VIRTUAL(UActionBase::OnActionDestroyed(), return TEXT("None"););

	virtual void CancelAction() {}

private:

protected:

	TArray<FName> Blacklist;
};