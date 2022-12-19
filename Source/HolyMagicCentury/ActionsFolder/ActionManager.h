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
	const bool IsCurrentAction(const FName& ActionName);

	void OnLanded(const FHitResult& Hit);

	using CreateActionFuncPtr = UActionBase* (*)(UActionDataBase* DataPtr, UObject* OuterObject);
	static TMap<FName, CreateActionFuncPtr> MapActionTypes;

	// Last Action Period.
	FORCEINLINE const FName& GetLastKnownAction() const { return LastKnownAction; }
	// Last Action that inherits Default (so basically any action we can idle back into)
	FORCEINLINE const FName& GetLastKnownDefaultAction() const { return LastKnownDefaultAction; }

private:

	bool CreateActionFromName(const FName& ActionName, UActionDataBase* Data);
	void SetCurrentAction(UActionBase* NewAction);

private:

	TWeakObjectPtr<UActionBase> CurrentAction;
	FName LastKnownAction;
	FName LastKnownDefaultAction;
};
