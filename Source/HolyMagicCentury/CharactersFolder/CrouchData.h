// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "CrouchData.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;

USTRUCT(BlueprintType)
struct FCrouchData
{
	GENERATED_BODY()

public:

	FCrouchData();
	FCrouchData(UCapsuleComponent* Capsule, USkeletalMeshComponent* SkeletalMesh);

	UPROPERTY(Transient)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Transient)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(Transient)
	bool bIsCrouched;

	UPROPERTY(Transient)
	float StandingHalfHeight;

	UPROPERTY(Transient)
	float CrouchedHalfHeight;

	UPROPERTY(Transient)
	float MeshOffsetZ;

	UPROPERTY(Transient)
	float LerpRatio;

	UPROPERTY(Transient)
	float CachedMeshLocationZ;

	void ToggleCrouch();
	void CrouchInstant();
	void UnCrouchInstant();
	void SetCrouched(bool Value);
	void LerpCrouch(float NewHalfHeight);
	void CacheMeshLocation();
};