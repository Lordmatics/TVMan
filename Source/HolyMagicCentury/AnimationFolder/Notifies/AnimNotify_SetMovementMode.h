// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotify_SetMovementMode : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotify_SetMovementMode();

private:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Character")
		TEnumAsByte<EMovementMode> MovementMode;
};
