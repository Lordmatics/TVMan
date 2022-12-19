// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ReduceCapsule.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UAnimNotifyState_ReduceCapsule : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_ReduceCapsule();

private:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UCapsuleComponent* GetCapsule(const USkeletalMeshComponent& MeshComp) const;
private:

	/*
	*	How much should the half height be scale by during the course of this animation.	
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float HalfHeightRatio;
};
