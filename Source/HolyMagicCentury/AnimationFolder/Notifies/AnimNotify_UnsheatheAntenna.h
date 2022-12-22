// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_UnsheatheAntenna.generated.h"

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotify_UnsheatheAntenna : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UAnimNotify_UnsheatheAntenna();

private:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = "Character")
	bool bReverse;

};
