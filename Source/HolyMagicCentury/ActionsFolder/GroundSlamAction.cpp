// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/GroundSlamAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "LeapAction.h"
#include "HideAction.h"
#include "JumpAction.h"
#include "ActionManager.h"
#include <DrawDebugHelpers.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "../ActorsFolder/DestructibleObject.h"

UGroundSlamActionData::UGroundSlamActionData()
{

}

UGroundSlamActionData::~UGroundSlamActionData()
{

}

void UGroundSlamActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

UGroundSlamAction::UGroundSlamAction()
{
	Blacklist.Push(ActionNames::LeapAction);
	Blacklist.Push(ActionNames::HideAction);
	Blacklist.Push(ActionNames::JumpAction);

	RegisterActionToManager(ActionNames::GroundSlamAction, UGroundSlamAction);
}

UGroundSlamAction::~UGroundSlamAction()
{

}

void UGroundSlamAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	UGroundSlamActionData* ActionData = Cast<UGroundSlamActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}
	
	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetGroundSlamming(true);
		}		
	}
}

void UGroundSlamAction::OnActionCreated()
{

}

void UGroundSlamAction::OnActionProcess(const float DeltaTime)
{
	//UObject* Owner = GetOuter();
	//if (!Owner)
	//{
	//	return;
	//}

	//UWorld* World = Owner->GetWorld();
	//if (!World)
	//{
	//	return;
	//}

	//ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner);
	//if (!BaseCharacter)
	//{
	//	return;
	//}

	//FHitResult HitResult;
	//const FVector& Start = BaseCharacter->GetActorLocation();
	//const float RayLength = 100.0f;
	//const FVector End = Start - FVector(0.0f, 0.0f, RayLength);
	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(BaseCharacter);
	//
	//const bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldDynamic, QueryParams);
	//	
	//bool bPersistent = true;
	//float LifeTime = 0.0f;
	//
	//// @fixme, draw line with thickness = 2.f?
	//if (bHit && HitResult.bBlockingHit)
	//{
	//	if (GEngine && HitResult.GetActor())
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
	//	}
	//		
	//	// Red up to the blocking hit, green thereafter
	//	DrawDebugLine(World, Start, HitResult.ImpactPoint, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	//	DrawDebugLine(World, HitResult.ImpactPoint, End, FLinearColor::Green.ToFColor(true), bPersistent, LifeTime);
	//	DrawDebugPoint(World, HitResult.ImpactPoint, 16.0f, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	//}
	//else
	//{
	//	// no hit means all red
	//	DrawDebugLine(World, Start, End, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	//}
	//
	//if (!bHit)
	//{
	//	return;
	//}

	//CancelAction();
}

void UGroundSlamAction::OnActionDestroyed()
{
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

	const float DefaultGravity = BaseCharacter->GetDefaultGravity();
	if (UCharacterMovementComponent* CharacterMovementComponent = BaseCharacter->GetCharacterMovement())
	{
		CharacterMovementComponent->GravityScale = DefaultGravity;
	}

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetGroundSlamming(false);
	}
}

void UGroundSlamAction::OnLanded(const FHitResult& HitResult)
{
	if (AActor* Actor = HitResult.GetActor())
	{
		if (ADestructibleObject* Destructible = Cast<ADestructibleObject>(Actor))
		{
			Destructible->Explode(HitResult.Location);
		}
	}

	CancelAction();
}

void UGroundSlamAction::CancelAction()
{
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

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetGroundSlamming(false);
	}

	BaseCharacter->EndAction();
}