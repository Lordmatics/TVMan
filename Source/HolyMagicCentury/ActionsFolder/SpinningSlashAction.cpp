// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/SpinningSlashAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "ActionManager.h"
#include "HolyMagicCentury/CharactersFolder/CharacterMontages.h"
#include <Kismet/KismetSystemLibrary.h>
#include <DrawDebugHelpers.h>
#include "LungeAction.h"
#include "JumpAction.h"
#include "HideAction.h"
#include "LeapAction.h"

USpinningSlashActionData::USpinningSlashActionData()
{

}

USpinningSlashActionData::~USpinningSlashActionData()
{

}

void USpinningSlashActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

USpinningSlashAction::USpinningSlashAction() :
	Timer(0.0f),
	RaycastInterval(0.1f),
	Counter(0),
	CollisionChecksUntilCompletion(40)
{
	Blacklist.Add(ActionNames::LungeAction);
	Blacklist.Add(ActionNames::LeapAction);
	Blacklist.Add(ActionNames::HideAction);
	Blacklist.Add(ActionNames::JumpAction);
	RegisterActionToManager(ActionNames::SpinningSlashAction, USpinningSlashAction);
}

USpinningSlashAction::~USpinningSlashAction()
{

}

void USpinningSlashAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	Super::InitialiseAction(ActionDataBase);

	USpinningSlashActionData* ActionData = Cast<USpinningSlashActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}

	Counter = 0;

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
		{
			AnimInstance->SetSpinningSlashing(true);
		}
	}
}

void USpinningSlashAction::OnActionCreated()
{
	Super::OnActionCreated();
}

void USpinningSlashAction::OnActionProcess(const float DeltaTime)
{
	Super::OnActionProcess(DeltaTime);

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

	UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	// Collision Queries at an interval.
	Timer += DeltaTime;
	if (Timer >= RaycastInterval)
	{
		Timer -= RaycastInterval;
		++Counter;
		const bool bHit = CheckForCollision(*BaseCharacter);
		if (bHit)
		{
			// Cancel collision detection ?
		}
	}
	
	if (Counter >= CollisionChecksUntilCompletion)
	{
		CancelAction();
	}
}

void USpinningSlashAction::OnActionDestroyed()
{
	Super::OnActionDestroyed();

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
		AnimInstance->SetSpinningSlashing(false);
	}
}

void USpinningSlashAction::OnLanded(const FHitResult& HitResult)
{
	Super::OnLanded(HitResult);
}

void USpinningSlashAction::CancelAction()
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
}

bool USpinningSlashAction::CheckForCollision(ABaseCharacter& BaseCharacter)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	FHitResult HitResult;

	FVector Start;
	if (!BaseCharacter.GetWeaponLocation(Start))
	{
		return false;
	}

	// NOTE: Potentially need socket location.
	const FVector& Forward = BaseCharacter.GetActorForwardVector();
	const float RayLength = 200.0f;
	FVector Length = FVector(0.0f, RayLength, 0.0f);
	const FVector End = Start + (Forward * Length);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(&BaseCharacter);

	TArray<AActor*> ActorsToIgnore;
	AActor* OwnerChar = &BaseCharacter;
	ActorsToIgnore.Add(OwnerChar);

	const float Radius = 50.0f;
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetOuter(), Start, End, Radius, ETraceTypeQuery::TraceTypeQuery2, true,
		ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);

	if (!bHit)
	{
		return false;
	}

	//const bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldDynamic, QueryParams);

	bool bPersistent = true;
	float LifeTime = 0.0f;
	if (bHit && HitResult.bBlockingHit)
	{
		if (GEngine && HitResult.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, World->GetDeltaSeconds(), FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
		}

		// Red up to the blocking hit, green thereafter
		DrawDebugLine(World, Start, HitResult.ImpactPoint, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
		DrawDebugLine(World, HitResult.ImpactPoint, End, FLinearColor::Green.ToFColor(true), bPersistent, LifeTime);
		DrawDebugPoint(World, HitResult.ImpactPoint, 16.0f, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);

		UAttackAction::DestroySmashable(HitResult.GetActor(), HitResult.ImpactPoint, HitResult.ImpactNormal, BaseCharacter.GetWeaponStrength());
		return true;
	}
	else
	{
		// no hit means all red
		DrawDebugLine(World, Start, End, FLinearColor::Red.ToFColor(true), bPersistent, LifeTime);
	}
	return false;
}
