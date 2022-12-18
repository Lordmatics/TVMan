// Lordmatics Games December 2022


#include "HolyMagicCentury/ActionsFolder/LeapAction.h"
#include "../CharactersFolder/BaseCharacter.h"
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>

ULeapAction::ULeapAction() :
	Super(),
	Timer(0.0f),
	ProcessDelayTime(0.8f)
{

}

ULeapAction::~ULeapAction()
{

}

void ULeapAction::InitialiseAction(UActionDataBase* ActionData)
{
	ULeapActionData* LeapData = Cast<ULeapActionData>(ActionData);
	if (!LeapData)
	{
		return;
	}

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetLeaping(true);
		}

		BaseCharacter->SetFlying(true);
	}
}

void ULeapAction::OnActionCreated()
{
	Timer = 0.0f;
}

void ULeapAction::OnActionProcess(const float DeltaTime)
{
	Timer += DeltaTime;
	if (Timer <= ProcessDelayTime)
	{
		return;
	}

	// Determine when the leap should finish. Do a raycast down
	UObject* Owner = GetOuter();
	if (!Owner)
	{
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return;
	}

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner);
	if (!BaseCharacter)
	{
		return;
	}

	FHitResult HitResult;
	const FVector& Start = BaseCharacter->GetActorLocation();
	const float RayLength = 100.0f;
	const FVector End = Start - FVector(0.0f, 0.0f, RayLength);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(BaseCharacter);

	const bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldDynamic, QueryParams);
	
	bool bPersistent = true;
	float LifeTime = 0.0f;

	// @fixme, draw line with thickness = 2.f?
	if (bHit && HitResult.bBlockingHit)
	{
		if (GEngine && HitResult.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
		}
		
		// Red up to the blocking hit, green thereafter
		DrawDebugLine(World, Start, HitResult.ImpactPoint, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
		DrawDebugLine(World, HitResult.ImpactPoint, End, FLinearColor::Green.ToFColor(true), bPersistent, LifeTime);
		DrawDebugPoint(World, HitResult.ImpactPoint, 16.0f, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	}
	else
	{
		// no hit means all red
		DrawDebugLine(World, Start, End, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	}

	if (!bHit)
	{
		return;
	}

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetLeaping(false);
	}
}

void ULeapAction::OnActionDestroyed()
{
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetLeaping(false);
		}

		BaseCharacter->SetFlying(false);
	}
}

ULeapActionData::ULeapActionData() :
	Super()
{

}

ULeapActionData::~ULeapActionData()
{

}

void ULeapActionData::InitialiseObject()
{
	Super::InitialiseObject();
}
