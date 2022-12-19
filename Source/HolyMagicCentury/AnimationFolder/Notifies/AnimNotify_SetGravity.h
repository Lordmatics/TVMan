// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetGravity.generated.h"

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotify_SetGravity : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UAnimNotify_SetGravity();

private:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Character", meta = (ClampMin = "0.01", EditCondition = "!bResetToDefault", EditConditionHides))
	float GravityScaleFactor;

	UPROPERTY(EditAnywhere, Category = "Character", meta = (ClampMin = "0.01"))
	bool bResetToDefault;
};
