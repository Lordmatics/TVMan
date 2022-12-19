// Lordmatics Games December 2022

#include "HolyMagicCentury/CharactersFolder/CrouchData.h"
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>

FCrouchData::FCrouchData() :
	CapsuleComponent(nullptr),
	Mesh(nullptr),
	bIsCrouched(false),
	StandingHalfHeight(96.0f),
	CrouchedHalfHeight(48.0f),
	MeshOffsetZ(48.0f),
	LerpRatio(0.0f),
	CachedMeshLocationZ(0.0f)
{

}

FCrouchData::FCrouchData(UCapsuleComponent* Capsule, USkeletalMeshComponent* SkeletalMesh) :
	CapsuleComponent(Capsule),
	Mesh(SkeletalMesh),
	bIsCrouched(false),
	LerpRatio(0.0f)
{
	if (!Capsule || !SkeletalMesh)
	{
		return;
	}

	StandingHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	CrouchedHalfHeight = StandingHalfHeight * 0.5f;
	MeshOffsetZ = StandingHalfHeight - CrouchedHalfHeight;
	CachedMeshLocationZ = SkeletalMesh->GetComponentLocation().Z;
}

void FCrouchData::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouchInstant();
	}
	else
	{
		CrouchInstant();
	}
}

void FCrouchData::CrouchInstant()
{
	if (!CapsuleComponent || !Mesh)
	{
		return;
	}

	FVector MeshLocation = Mesh->GetComponentLocation();
	MeshLocation.Z += MeshOffsetZ;
	Mesh->SetWorldLocation(MeshLocation);
	CapsuleComponent->SetCapsuleHalfHeight(CrouchedHalfHeight, true);
	SetCrouched(true);
}

void FCrouchData::UnCrouchInstant()
{
	if (!CapsuleComponent || !Mesh)
	{
		return;
	}

	CapsuleComponent->SetCapsuleHalfHeight(StandingHalfHeight, true);
	FVector MeshLocation = Mesh->GetComponentLocation();
	MeshLocation.Z -= MeshOffsetZ;
	Mesh->SetWorldLocation(MeshLocation);
	SetCrouched(false);
}

void FCrouchData::SetCrouched(bool Value)
{
	bIsCrouched = Value;
}

void FCrouchData::LerpCrouch(float NewHalfHeight)
{
	if (!CapsuleComponent || !Mesh)
	{
		return;
	}

	int OffsetMod = 1;
	float LerpedMeshOffsetZ = MeshOffsetZ;
	if (bIsCrouched)
	{
		// Adding
		OffsetMod = -1;
		// 48, blending to 96
		// so 50 - 48 = 2
		LerpedMeshOffsetZ = NewHalfHeight - CrouchedHalfHeight;
	}
	else
	{
		// Subtracting
		LerpedMeshOffsetZ = StandingHalfHeight - NewHalfHeight;
	}
	
	//LerpedMeshOffsetZ *= 2;

	CapsuleComponent->SetCapsuleHalfHeight(NewHalfHeight, false);
	float StartZLocation = CachedMeshLocationZ;
	StartZLocation += (LerpedMeshOffsetZ * OffsetMod);
	FVector MeshLocation = Mesh->GetRelativeLocation();
	MeshLocation.Z = StartZLocation;
	Mesh->SetRelativeLocation(MeshLocation);
}

void FCrouchData::CacheMeshLocation()
{
	if (!CapsuleComponent || !Mesh)
	{
		return;
	}

	CachedMeshLocationZ = Mesh->GetRelativeLocation().Z;
}
