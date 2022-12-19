// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotifyState_ReduceCapsule.h"
#include <Components/CapsuleComponent.h>
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"

UAnimNotifyState_ReduceCapsule::UAnimNotifyState_ReduceCapsule() :
	Super(),
	HalfHeightRatio(1.0f)
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
	CrouchData.ToggleCrouch();	
}

void UAnimNotifyState_ReduceCapsule::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (!MeshComp)
	{
		return;
	}

	//UCapsuleComponent* CapsuleComponent = GetCapsule(*MeshComp);
	//if (!CapsuleComponent)
	//{
	//	return;
	//}

	//// Increase Mesh Z by the same amount.
	//PreviousCapsuleHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	//const float NewHalfHeight = PreviousCapsuleHeight * 0.5f;

	//FVector MeshLocation = MeshComp->GetComponentLocation();
	//MeshLocation.Z += NewHalfHeight;
	//MeshComp->SetWorldLocation(MeshLocation);

	//Capsule->SetCapsuleHalfHeight(NewHalfHeight, true);
	//// Blend HalfHeight between two values.
	//// Adjust Mesh by the same ratio.
}

void UAnimNotifyState_ReduceCapsule::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
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
