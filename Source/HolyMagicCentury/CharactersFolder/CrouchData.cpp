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
	MeshOffsetZ(48.0f)
{

}

FCrouchData::FCrouchData(UCapsuleComponent* Capsule, USkeletalMeshComponent* SkeletalMesh) :
	CapsuleComponent(Capsule),
	Mesh(SkeletalMesh),
	bIsCrouched(false)
{
	if (!Capsule || !SkeletalMesh)
	{
		return;
	}

	StandingHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	CrouchedHalfHeight = StandingHalfHeight * 0.5f;
	MeshOffsetZ = StandingHalfHeight - CrouchedHalfHeight;
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
