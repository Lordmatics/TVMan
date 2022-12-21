// Lordmatics Games December 2022


#include "HolyMagicCentury/ActionsFolder/LeapAction.h"
#include "../CharactersFolder/BaseCharacter.h"
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#include "HideAction.h"
#include "JumpAction.h"
#include "ActionManager.h"
#include "../AnimationFolder/Notifies/AnimNotify_RaycastDown.h"
#include <Components/CapsuleComponent.h>
#include "../ActorsFolder/LeapFlap.h"

ULeapAction::ULeapAction() :
	Super(),
	Timer(0.0f),
	ProcessDelayTime(0.8f),
	Expiration(7.5f)
{
	Blacklist.Push(ActionNames::JumpAction);
	Blacklist.Push(ActionNames::HideAction);
	
	RegisterActionToManager(ActionNames::LeapAction, ULeapAction);
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

		if (BaseCharacter->GetLastKnownDefaultActionName() == ActionNames::HideAction)
		{

		}
		else
		{
			// Only TV Man needs to fly when leaping.
			BaseCharacter->SetFlying(true);
		}		
	}
}

void ULeapAction::OnActionCreated()
{
	Timer = 0.0f;

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		const FName& LastKnownDefaultActionNme = BaseCharacter->GetLastKnownDefaultActionName();
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			if (LastKnownDefaultActionNme == ActionNames::HideAction)
			{
				AnimInstance->SetHiding(true);
			}
		}

		if (LastKnownDefaultActionNme == ActionNames::HideAction)
		{
			
		}
		else
		{
			// Only TV Man needs to fly when leaping.
			BaseCharacter->SetFlying(true);
		}		
	}

	UAnimNotify_RaycastDown::OnCollisionDetected.AddDynamic(this, &ULeapAction::OnRaycastHit);
}

void ULeapAction::OnActionProcess(const float DeltaTime)
{
	Timer += DeltaTime;

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		CheckCollisions(*BaseCharacter);
	}

	if (Timer <= ProcessDelayTime)
	{
		return;
	}

	if (Timer >= Expiration)
	{
		CancelAction();
		return;
	}

	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetLeaping(true);
		}
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

	UAnimNotify_RaycastDown::OnCollisionDetected.RemoveDynamic(this, &ULeapAction::OnRaycastHit);
}

void ULeapAction::OnLanded(const FHitResult& Hit)
{
	// Determine when the leap should finish. Do a raycast down
	CancelAction();
}

void ULeapAction::CancelAction()
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

	BaseCharacter->EndAction();

	if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
	{
		AnimInstance->SetLeaping(false);
	}
}

void ULeapAction::OnRaycastHit()
{
	CancelAction();
}

void ULeapAction::CheckCollisions(const ABaseCharacter& BaseCharacter)
{
	// HERE:
	UCapsuleComponent* CapsuleComponent = BaseCharacter.GetCapsuleComponent();
	if (!CapsuleComponent)
	{
		return;
	}

	UWorld* World = BaseCharacter.GetWorld();
	if (!World)
	{
		return;
	}

	FHitResult HitResult;
	const FVector& Start = CapsuleComponent->GetComponentLocation();
	const FVector& Forward = BaseCharacter.GetActorForwardVector();
	const float RayLength = 300.0f;
	FVector Length = FVector(0.0f, RayLength, -50.0f);
	const FVector End = Start + (Forward * Length);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(&BaseCharacter);

	const bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldDynamic, QueryParams);

	bool bPersistent = true;
	float LifeTime = 0.0f;

	// @fixme, draw line with thickness = 2.f?
	if (bHit && HitResult.bBlockingHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (GEngine)
			{
				const float DeltaTime = World->GetDeltaSeconds();
				GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
			}

			const FVector RayDirection = (End - Start).GetUnsafeNormal();
			if (ALeapFlap* LeapFlap = Cast<ALeapFlap>(HitActor))
			{
				// Compare Dot Product between leapflap forward vector and hit ray direction
				const FVector& LeapFlapForwardVec = LeapFlap->GetActorForwardVector();
				const float Dot = LeapFlapForwardVec.DotProduct(LeapFlapForwardVec, RayDirection);
				if (Dot <= 0.0f)
				{
					LeapFlap->OpenBack();
				}
				else
				{
					LeapFlap->OpenFront();
				}
			}
		}
		

		// Red up to the blocking hit, green thereafter
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
