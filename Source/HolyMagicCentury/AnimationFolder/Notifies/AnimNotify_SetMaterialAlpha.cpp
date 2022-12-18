// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotify_SetMaterialAlpha.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"

UAnimNotify_SetMaterialAlpha::UAnimNotify_SetMaterialAlpha() :
	Super(),
	AlphaValue(1.0f)
{

}

void UAnimNotify_SetMaterialAlpha::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{	
	if (!MeshComp)
	{
		return;
	}

	AActor* MeshOwner = MeshComp->GetOwner();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshOwner))
	{
		BaseCharacter->SetBodyAlpha(AlphaValue);
	}
}
