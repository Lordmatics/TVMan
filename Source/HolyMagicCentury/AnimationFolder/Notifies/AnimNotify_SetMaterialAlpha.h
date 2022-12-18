// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetMaterialAlpha.generated.h"

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotify_SetMaterialAlpha : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UAnimNotify_SetMaterialAlpha();

private:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Character", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AlphaValue;

};
