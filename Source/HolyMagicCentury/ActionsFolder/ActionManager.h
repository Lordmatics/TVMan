// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "ActionManager.generated.h"

class UActionBase;
class UActionDataBase;

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UActionManager : public UObject
{
public:

	GENERATED_BODY()

	UActionManager();
	~UActionManager();

	void InitialiseManager(UObject* Owner);

	bool RequestAction(const FName& ActionName, UActionDataBase* Data);

	bool Process(const float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void EndCurrentAction();

	const FString GetCurrentActionName() const;
	const bool IsCurrentAnimation(const FName& ActionName);

private:

	bool CreateActionFromName(const FName& ActionName, UActionDataBase* Data);
	void SetCurrentAction(UActionBase* NewAction);

private:

	TWeakObjectPtr<UActionBase> CurrentAction;
};
