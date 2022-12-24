// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"

UBaseCharacterAnimationInstance::UBaseCharacterAnimationInstance() :
	Super(),
	bIsHiding(false),
	bIsLeaping(false),
	bIsGroundSlamming(false),
	bUnsheatheAntenna(false),
	bSheatheAntenna(false),
	bAttacking(false),
	bLunging(false),
	bSlashing(false),
	bSpinningSlashing(false),
	bUseTorso(false)
{

}

void UBaseCharacterAnimationInstance::ResetNonDefaults()
{
	// Reset Movement actions.
	SetLeaping(false);
	SetGroundSlamming(false);	
}
