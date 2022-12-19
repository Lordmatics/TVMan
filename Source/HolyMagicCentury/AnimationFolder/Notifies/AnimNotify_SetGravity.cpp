// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotify_SetGravity.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

UAnimNotify_SetGravity::UAnimNotify_SetGravity() :
	GravityScaleFactor(1.0f),
	bResetToDefault(false)
{

}

void UAnimNotify_SetGravity::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	const float DefaultGravity = BaseCharacter->GetDefaultGravity();
	UCharacterMovementComponent* CharacterMovementComponent = BaseCharacter->GetCharacterMovement();
	if (!CharacterMovementComponent)
	{
		return;
	}

	if (bResetToDefault)
	{
		CharacterMovementComponent->GravityScale = DefaultGravity;
	}
	else
	{
		CharacterMovementComponent->GravityScale *= GravityScaleFactor;
	}	
}
