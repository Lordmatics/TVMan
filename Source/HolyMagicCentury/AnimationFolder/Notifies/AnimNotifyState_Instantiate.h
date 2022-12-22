// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Instantiate.generated.h"

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotifyState_Instantiate : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UAnimNotifyState_Instantiate();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:

	UPROPERTY(EditAnywhere, Category = "Character")
		TSubclassOf<AActor> ObjectToInstantiate;

	UPROPERTY(EditAnywhere, Category = "Character")
		FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, Category = "Character")
		FName SocketToAttachTo;
	
	TWeakObjectPtr<AActor> WeakSpawnedObject;
};
