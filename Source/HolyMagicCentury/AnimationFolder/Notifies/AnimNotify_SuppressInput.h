// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SuppressInput.generated.h"

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotify_SuppressInput : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotify_SuppressInput();

private:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Character")
		bool  bSuppressMovement;

	UPROPERTY(EditAnywhere, Category = "Character")
		bool bSuppressRotation;
};
