// Lordmatics Games December 2022
#include "HolyMagicCentury/ActionsFolder/SlashAction.h"
#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include "HolyMagicCentury/AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Engine/World.h>
#include "ActionManager.h"
#include "../CharactersFolder/CharacterMontages.h"
#include <Kismet/KismetSystemLibrary.h>
#include <DrawDebugHelpers.h>
#include "LungeAction.h"

USlashActionData::USlashActionData()
{

}

USlashActionData::~USlashActionData()
{

}

void USlashActionData::InitialiseObject()
{
	Super::InitialiseObject();
}

USlashAction::USlashAction() :
	Timer(0.0f),
	RaycastInterval(0.1f)
{
	Blacklist.Add(ActionNames::LungeAction);

	RegisterActionToManager(ActionNames::SlashAction, USlashAction);
}

USlashAction::~USlashAction()
{

}

void USlashAction::InitialiseAction(UActionDataBase* ActionDataBase)
{
	Super::InitialiseAction(ActionDataBase);

	USlashActionData* ActionData = Cast<USlashActionData>(ActionDataBase);
	if (!ActionData)
	{
		return;
	}

	UObject* Owner = GetOuter();
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Owner))
	{
		FCharacterMontages& MontagePacket = BaseCharacter->GetMontagePacket();
		UAnimMontage* Montage = MontagePacket.SlashMontage;
		if (!Montage)
		{
			return;
		}

		const float Duration = BaseCharacter->PlayAnimMontage(Montage, 1.0f, NAME_None);
		if (Duration >= 0.0f)
		{
			if (UBaseCharacterAnimationInstance* AnimInstance = BaseCharacter->GetAnimInstance())
			{
				AnimInstance->SetSlashing(true);
			}
		}
	}
}

void USlashAction::OnActionCreated()
{
	Super::OnActionCreated();
}

void USlashAction::OnActionProcess(const float DeltaTime)
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

	FCharacterMontages& MontagePacket = BaseCharacter->GetMontagePacket();
	UAnimMontage* Montage = MontagePacket.SlashMontage;
	if (!Montage)
	{
		return;
	}

	const bool bIsPlaying = AnimInstance->Montage_IsPlaying(Montage);	
	if (!bIsPlaying)
	{
		CancelAction();
	}
	else
	{
		// Collision Queries at an interval.
		Timer += DeltaTime;
		if (Timer >= RaycastInterval)
		{
			Timer -= RaycastInterval;
			const bool bHit = CheckForCollision(*BaseCharacter);
			if (bHit)
			{
				// Cancel collision detection ?
			}
		}
	}
}

void USlashAction::OnActionDestroyed()
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
		AnimInstance->SetLunging(false);
	}
}

void USlashAction::OnLanded(const FHitResult& HitResult)
{
	Super::OnLanded(HitResult);
}

void USlashAction::CancelAction()
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

bool USlashAction::CheckForCollision(ABaseCharacter& BaseCharacter)
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
