// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotify_SuppressInput.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"

UAnimNotify_SuppressInput::UAnimNotify_SuppressInput() :
	bSuppressMovement(false),
	bSuppressRotation(false)
{

}

void UAnimNotify_SuppressInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	AActor* MeshOwner = MeshComp->GetOwner();
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshOwner);
	if (!BaseCharacter)
	{
		return;
	}

	BaseCharacter->SetMovementDisabled(bSuppressMovement);
	BaseCharacter->SetRotationDisabled(bSuppressRotation);
}
