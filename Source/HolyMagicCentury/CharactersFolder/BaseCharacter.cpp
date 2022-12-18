// Lordmatics Games December 2022


#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Materials/MaterialInstanceDynamic.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->InitCapsuleSize(42.0f, 96.0f);
	}

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	if (UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement())
	{
		MyCharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
		MyCharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
		MyCharacterMovement->JumpZVelocity = 600.f;
		MyCharacterMovement->AirControl = 0.2f;
	}

	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
	ThirdPersonSpringArm->SetupAttachment(RootComponent);
	ThirdPersonSpringArm->TargetArmLength = 300.0f;
	ThirdPersonSpringArm->bUsePawnControlRotation = true;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm, USpringArmComponent::SocketName);	
	ThirdPersonCamera->bUsePawnControlRotation = false;

}

void ABaseCharacter::Jump()
{
	ACharacter::Jump();
}

void ABaseCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void ABaseCharacter::OnActionPressed()
{
	UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	const bool bIsHiding = AnimInstance->IsHiding();
	SetHiding(!bIsHiding);
}

void ABaseCharacter::OnActionReleased()
{

}

void ABaseCharacter::SetBodyAlpha(float Value)
{
	const int NumMaterials = DynamicMaterials.Num();
	if (NumMaterials <= 1)
	{
		return;
	}

	UMaterialInstanceDynamic* BodyA = DynamicMaterials[0];
	if (BodyA)
	{
		BodyA->SetScalarParameterValue(MaterialParameterNames::Alpha, Value);
	}

	UMaterialInstanceDynamic* BodyB = DynamicMaterials[1];
	if (BodyB)
	{
		BodyB->SetScalarParameterValue(MaterialParameterNames::Alpha, Value);
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (DynamicMaterials.Num() > 0)
	{
		DynamicMaterials.Empty();
	}	

	// Create Material Instance.
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (!SkeletalMesh)
	{
		return;
	}

	const TArray<UMaterialInterface*>& Materials = SkeletalMesh->GetMaterials();
	int Index = 0;
	for (UMaterialInterface* Material : Materials)
	{
		if (!Material)
		{
			++Index;
			continue;
		}

		if (UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this))
		{
			DynamicMaterials.Push(DynamicMaterial);
			SkeletalMesh->SetMaterial(Index, DynamicMaterial);
		}
		++Index;
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCharacterMovementComponent* CharMover = GetCharacterMovement();
	if (!CharMover)
	{
		ResetAnimationMovement();
		return;
	}

	const FVector& Velocity = CharMover->GetLastUpdateVelocity();
	const float Speed = Velocity.Size();

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Speed: %.1f"), Speed));
	if (Speed > 0.0f)
	{
		//if (Speed >= 0.5f)
		//{
		//	SetRunning(true);
		//}
		//else
		
		{
			SetWalking(true);
		}
	}
	else
	{
		ResetAnimationMovement();
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ABaseCharacter::OnActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ABaseCharacter::OnActionReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);
}

void ABaseCharacter::MoveInDirection(EAxis::Type Axis, const float Value)
{	
	if (!Controller)
	{
		ResetAnimationMovement();
		return;
	}

	if (Value == 0.0f)
	{
		ResetAnimationMovement();
		return;
	}

	// find out which way is forward
	const FRotator& Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(Axis);
	AddMovementInput(Direction, Value);
	OnMovementRequested(Value);
}

void ABaseCharacter::MoveForward(float Value)
{
	MoveInDirection(EAxis::X, Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	MoveInDirection(EAxis::Y, Value);	
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float DeltaSeconds = World->GetDeltaSeconds();
	const float YawInput = Rate * BaseTurnRate * DeltaSeconds;
	AddControllerYawInput(YawInput);
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}


	const float DeltaSeconds = World->GetDeltaSeconds();
	const float PitchInput = Rate * BaseLookUpRate * DeltaSeconds;

	//GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Red, FString::Printf(TEXT("LookUpRate: %.1f"), PitchInput));

	AddControllerPitchInput(PitchInput);
}

void ABaseCharacter::AddControllerYawInput(float Value)
{
	APawn::AddControllerYawInput(Value);
}

void ABaseCharacter::AddControllerPitchInput(float Value)
{
	Value *= -1.0f;
	APawn::AddControllerPitchInput(Value);
}

UBaseCharacterAnimationInstance* ABaseCharacter::GetAnimInstance() const
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (!SkeletalMesh)
	{
		return nullptr;
	}

	UBaseCharacterAnimationInstance* AnimInstance = Cast<UBaseCharacterAnimationInstance>(SkeletalMesh->GetAnimInstance());
	return AnimInstance;
}

void ABaseCharacter::OnMovementRequested(float Value)
{
	UCharacterMovementComponent* CharMover = GetCharacterMovement();
	if (!CharMover)
	{
		ResetAnimationMovement();
		return;
	}

	//const FVector& Velocity = CharMover->GetLastUpdateVelocity();
	//const float Speed = Velocity.Size();
	//
	//if (Speed > 0.0f)
	//{
	//	if (Speed >= 0.5f)
	//	{
	//		SetRunning(true);
	//	}
	//	else
	//	{
	//		SetWalking(true);
	//	}
	//}
	//else
	//{
	//	ResetAnimationMovement();
	//}
}

void ABaseCharacter::SetWalking(bool Value)
{
	UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->SetWalking(Value);
}

void ABaseCharacter::SetRunning(bool Value)
{
	UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->SetRunning(Value);
}

void ABaseCharacter::SetHiding(bool Value)
{
	UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->SetHiding(Value);
}

void ABaseCharacter::ResetAnimationMovement()
{
	SetWalking(false);
	SetRunning(false);
}

