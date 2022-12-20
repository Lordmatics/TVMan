// Lordmatics Games December 2022


#include "HolyMagicCentury/ActorsFolder/LeapFlap.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimMontage.h>

// Sets default values
ALeapFlap::ALeapFlap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	LeapFlapRim = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeapFlapRim"));
	LeapFlapRim->SetupAttachment(Root);

	LeapFlap = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeapFlap"));
	LeapFlap->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ALeapFlap::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ALeapFlap::IsMontagePlaying() const
{
	if (!LeapFlap)
	{
		return false;
	}

	const UAnimInstance* AnimInstance = LeapFlap->GetAnimInstance();
	if (!AnimInstance)
	{
		return false;
	}

	const bool bAnyMontagePlaying = AnimInstance->Montage_IsPlaying(nullptr);
	return bAnyMontagePlaying;
}

bool ALeapFlap::PlayAnimMontage(UAnimMontage* Montage)
{
	if (!LeapFlap)
	{
		return false;
	}

	if (!Montage)
	{
		return false;
	}

	UAnimInstance* AnimInstance = LeapFlap->GetAnimInstance();
	if (!AnimInstance)
	{
		return false;
	}
	
	// Disable Collision
	LeapFlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	float const Duration = AnimInstance->Montage_Play(Montage, 1.0f);	
	return Duration > 0.0f;
}

void ALeapFlap::RestoreCollision()
{
	if (!LeapFlap)
	{
		return;
	}

	LeapFlap->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called every frame
void ALeapFlap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeapFlap::OpenFront()
{
	if (IsMontagePlaying())
	{
		return;
	}

	const float Duration = PlayAnimMontage(OpenFlapFrontAnimMontage);
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RestoreCollisionHandle);
		
		World->GetTimerManager().SetTimer(RestoreCollisionHandle, this, &ALeapFlap::RestoreCollision, Duration, false);
	}
}

void ALeapFlap::OpenBack()
{
	if (IsMontagePlaying())
	{
		return;
	}

	const float Duration = PlayAnimMontage(OpenFlapBackAnimMontage);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RestoreCollisionHandle);
		World->GetTimerManager().SetTimer(RestoreCollisionHandle, this,  &ALeapFlap::RestoreCollision, Duration, false);
	}	
}

