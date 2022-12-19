// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotify_SetMovementMode.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"

UAnimNotify_SetMovementMode::UAnimNotify_SetMovementMode() :
	MovementMode(EMovementMode::MOVE_Walking)
{

}

void UAnimNotify_SetMovementMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

	
	UCharacterMovementComponent* CharacterMovementComponent = BaseCharacter->GetCharacterMovement();
	if (!CharacterMovementComponent)
	{
		return;
	}

	if (MovementMode == MOVE_Walking)
	{
		CharacterMovementComponent->SetDefaultMovementMode();
	}
	else
	{
		CharacterMovementComponent->SetMovementMode(MovementMode);
	}
}
