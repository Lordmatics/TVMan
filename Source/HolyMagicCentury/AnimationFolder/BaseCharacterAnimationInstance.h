// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOLYMAGICCENTURY_API UBaseCharacterAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UBaseCharacterAnimationInstance();

	FORCEINLINE void SetHiding(bool Value) { bIsHiding = Value; }
	FORCEINLINE bool IsHiding() const { return bIsHiding; }

	FORCEINLINE void SetLeaping(bool Value) { bIsLeaping = Value; }
	FORCEINLINE bool IsLeaping() const { return bIsLeaping; }
	
	FORCEINLINE void SetGroundSlamming(bool Value) { bIsGroundSlamming = Value; }
	FORCEINLINE bool IsGroundSlamming() const { return bIsGroundSlamming; }

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsHiding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsLeaping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsGroundSlamming;
};
