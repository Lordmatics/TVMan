// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotifyState_Instantiate.h"

UAnimNotifyState_Instantiate::UAnimNotifyState_Instantiate() :
	ObjectToInstantiate(),
	SpawnRotation(FRotator::ZeroRotator),
	SocketToAttachTo(NAME_None),
	WeakSpawnedObject(nullptr)
{

}

void UAnimNotifyState_Instantiate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!MeshComp)
	{
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}

	FVector Location;	
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = MeshComp->GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* SpawnedActor = World->SpawnActor<AActor>(ObjectToInstantiate, Location, SpawnRotation, SpawnParameters);
	if (!SpawnedActor)
	{
		return;
	}

	WeakSpawnedObject = SpawnedActor;

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	SpawnedActor->AttachToComponent(MeshComp, AttachmentRules, SocketToAttachTo);	
}

void UAnimNotifyState_Instantiate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UAnimNotifyState_Instantiate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AActor* SpawnedObjectActor = WeakSpawnedObject.Get())
	{
		SpawnedObjectActor->Destroy();
	}
}
