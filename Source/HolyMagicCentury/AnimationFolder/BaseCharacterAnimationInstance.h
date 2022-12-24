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

	FORCEINLINE void SetUnsheatheAntenna(bool Value) { bUnsheatheAntenna = Value; }
	FORCEINLINE bool IsAntennaUnsheathed() const { return bUnsheatheAntenna; }

	FORCEINLINE void SetSheatheAntenna(bool Value) { bSheatheAntenna = Value; }
	FORCEINLINE bool IsAntennaSheathed() const { return bSheatheAntenna; }

	FORCEINLINE void SetAttacking(bool Value) { bAttacking = Value; }
	FORCEINLINE bool IsAttacking() const { return bAttacking; }

	FORCEINLINE void SetLunging(bool Value) { bLunging = Value; }
	FORCEINLINE bool IsLunging() const { return bLunging; }

	FORCEINLINE void SetSlashing(bool Value) { bSlashing = Value; }
	FORCEINLINE bool IsSlashing() const { return bSlashing; }

	FORCEINLINE void SetTorso(bool Value) { bUseTorso = Value; }
	FORCEINLINE bool IsTorso() const { return bUseTorso; }

	FORCEINLINE void SetSpinningSlashing(bool Value) { bSpinningSlashing = Value; }
	FORCEINLINE bool IsSpinningSlashing() const { return bSpinningSlashing; }

	void ResetNonDefaults();

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsHiding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsLeaping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsGroundSlamming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bUnsheatheAntenna;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	bool bSheatheAntenna;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bLunging;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bSlashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bSpinningSlashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		bool bUseTorso;
};
