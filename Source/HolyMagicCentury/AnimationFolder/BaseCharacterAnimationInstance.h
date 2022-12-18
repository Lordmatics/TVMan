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

	FORCEINLINE void SetWalking(bool Value) { bIsWalking = Value; }
	FORCEINLINE void SetRunning(bool Value) { bIsRunning = Value; }
	FORCEINLINE void SetHiding(bool Value) { bIsHiding = Value; }
	FORCEINLINE bool IsHiding() const { return bIsHiding; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsWalking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsRunning;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsHiding;
};
