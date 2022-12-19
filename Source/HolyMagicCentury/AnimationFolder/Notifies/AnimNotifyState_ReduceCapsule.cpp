// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotifyState_ReduceCapsule.h"
#include <Components/CapsuleComponent.h>
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"

UAnimNotifyState_ReduceCapsule::UAnimNotifyState_ReduceCapsule() :
	Super(),
	LerpSpeed(3.0f)
{

}

void UAnimNotifyState_ReduceCapsule::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);	

	if (!MeshComp)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!BaseCharacter)
	{
		return;
	}

	FCrouchData& CrouchData = BaseCharacter->GetCrouchData();
	CrouchData.LerpRatio = 0.0f;
	CrouchData.CacheMeshLocation();
}

void UAnimNotifyState_ReduceCapsule::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (!MeshComp)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!BaseCharacter)
	{
		return;
	}

	FCrouchData& CrouchData = BaseCharacter->GetCrouchData();
	float& LerpRatio = CrouchData.LerpRatio;
	const float CrouchedHalfHeight = CrouchData.CrouchedHalfHeight;
	const float StandingHalfHeight = CrouchData.StandingHalfHeight;
	
	if (CrouchData.bIsCrouched)
	{
		// Lerp from crouched -> uncrouched.
		LerpRatio += FrameDeltaTime * LerpSpeed;
		LerpRatio = FMath::Clamp(LerpRatio, 0.0f, 1.0f);
		float NewHalfHeight = FMath::Lerp(CrouchedHalfHeight, StandingHalfHeight, LerpRatio);
		CrouchData.LerpCrouch(NewHalfHeight);
	}
	else
	{
		// Lerp from uncrouched to crouched.
		LerpRatio += FrameDeltaTime * LerpSpeed;
		LerpRatio = FMath::Clamp(LerpRatio, 0.0f, 1.0f);
		float NewHalfHeight = FMath::Lerp(StandingHalfHeight, CrouchedHalfHeight, LerpRatio);
		CrouchData.LerpCrouch(NewHalfHeight);
	}
}

void UAnimNotifyState_ReduceCapsule::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (!MeshComp)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!BaseCharacter)
	{
		return;
	}

	FCrouchData& CrouchData = BaseCharacter->GetCrouchData();
	if (CrouchData.bIsCrouched)
	{
		CrouchData.SetCrouched(false);
	}
	else
	{
		CrouchData.SetCrouched(true);
	}
}

UCapsuleComponent* UAnimNotifyState_ReduceCapsule::GetCapsule(const USkeletalMeshComponent& MeshComp) const
{
	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp.GetOwner());
	if (!BaseCharacter)
	{
		return nullptr;
	}

	UCapsuleComponent* CapsuleComponent = BaseCharacter->GetCapsuleComponent();
	return CapsuleComponent;
}
