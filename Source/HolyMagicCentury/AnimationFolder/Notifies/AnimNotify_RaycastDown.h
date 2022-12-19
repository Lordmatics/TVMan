// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_RaycastDown.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionDetected);
/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotify_RaycastDown : public UAnimNotify
{
	GENERATED_BODY()

public:

	UAnimNotify_RaycastDown();

	static FOnCollisionDetected OnCollisionDetected;

private:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Character")
	float ZOffset;

	UPROPERTY(EditAnywhere, Category = "Character")
	float RayLength;
};
