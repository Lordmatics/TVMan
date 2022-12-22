// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotify_UnsheatheAntenna.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"

UAnimNotify_UnsheatheAntenna::UAnimNotify_UnsheatheAntenna() :
	bReverse(false)
{

}

void UAnimNotify_UnsheatheAntenna::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!BaseCharacter)
	{
		return;
	}

	if (bReverse)
	{
		BaseCharacter->DetachAntennaFromHand();
	}
	else
	{
		BaseCharacter->AttachAntennaToHand();
	}
}
