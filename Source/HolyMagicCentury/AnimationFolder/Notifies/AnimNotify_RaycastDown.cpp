// Lordmatics Games December 2022


#include "HolyMagicCentury/AnimationFolder/Notifies/AnimNotify_RaycastDown.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include <DrawDebugHelpers.h>

FOnCollisionDetected UAnimNotify_RaycastDown::OnCollisionDetected;

UAnimNotify_RaycastDown::UAnimNotify_RaycastDown() :
	ZOffset(0.0f),
	RayLength(100.0f)
{

}

void UAnimNotify_RaycastDown::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// Shoot Ray down, if it hits something, send event.

	if (!MeshComp)
	{
		return;
	}

	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!BaseCharacter)
	{
		return;
	}

	FHitResult HitResult;
	FVector Start = BaseCharacter->GetActorLocation();
	Start.Z += ZOffset;

	const FVector End = Start - FVector(0.0f, 0.0f, RayLength);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(BaseCharacter);
	QueryParams.AddIgnoredActor(MeshComp->GetOwner());

	const bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldDynamic, QueryParams);
		
	bool bPersistent = true;
	float LifeTime = 0.0f;
	
	// @fixme, draw line with thickness = 2.f?
	if (bHit && HitResult.bBlockingHit)
	{
		if (GEngine && HitResult.GetActor())
		{
			const float DeltaTime = World->GetDeltaSeconds();
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
		}
			
		//// Red up to the blocking hit, green thereafter
		//DrawDebugLine(World, Start, HitResult.ImpactPoint, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
		//DrawDebugLine(World, HitResult.ImpactPoint, End, FLinearColor::Green.ToFColor(true), bPersistent, LifeTime);
		//DrawDebugPoint(World, HitResult.ImpactPoint, 16.0f, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	}
	else
	{
		// no hit means all red
		//DrawDebugLine(World, Start, End, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	}
	
	if (!bHit)
	{
		return;
	}

	OnCollisionDetected.Broadcast();	
}
