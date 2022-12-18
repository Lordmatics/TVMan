// Lordmatics Games December 2022


#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Materials/MaterialInstanceDynamic.h>

// Sets default values
ABaseCharacter::ABaseCharacter() :
	WalkSpeed(300.0f),
	RunSpeed(600.0f),
	JumpVelocity(150.0f),
	ManJumpVelocity(150.0f),
	TVJumpVelocity(300.0f)
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
		//MyCharacterMovement->Acceleration = 1000.0f; // Set this in the BP.
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
	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		const bool bIsFalling = CharacterMovementComp->IsFalling();
		if (bIsFalling)
		{
			return;
		}
	}

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

	if (AnimInstance->IsLeaping())
	{
		return;
	}

	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		const bool bIsFalling = CharacterMovementComp->IsFalling();
		if (bIsFalling)
		{
			return;
		}
	}

	const bool bIsHiding = AnimInstance->IsHiding();
	SetHiding(!bIsHiding);
}

void ABaseCharacter::OnActionReleased()
{

}

void ABaseCharacter::OnLeapPressed()
{
	UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (AnimInstance->IsHiding())
	{
		return;
	}

	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		const bool bIsFalling = CharacterMovementComp->IsFalling();
		if (bIsFalling)
		{
			return;
		}
	}	

	SetLeaping(true);

	// Add Impulse
	//MoveForward(RunSpeed);
	
	//const FVector ImpulseVector = GetActorForwardVector() * 1000.0f;
	
}

void ABaseCharacter::OnLeapReleased()
{
	// Might need to push this onto a timer.
	// Or a callback on the anim notify?
	SetLeaping(false);
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

	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		CharacterMovementComp->MaxWalkSpeed = RunSpeed;
	}	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ABaseCharacter::OnActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ABaseCharacter::OnActionReleased);

	PlayerInputComponent->BindAction("Leap", IE_Pressed, this, &ABaseCharacter::OnLeapPressed);
	PlayerInputComponent->BindAction("Leap", IE_Released, this, &ABaseCharacter::OnLeapReleased);

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
		return;
	}

	if (Value == 0.0f)
	{
		return;
	}

	// find out which way is forward
	const FRotator& Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(Axis);

	const FVector& Velocity = GetVelocity();
	const float VectorLength = Velocity.Size();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.33f, FColor::Red, FString::Printf(TEXT("Vel: %.1f"), Value));
	}	
	AddMovementInput(Direction, Value);
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

void ABaseCharacter::SetHiding(bool Value)
{
	UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->SetHiding(Value);

	JumpVelocity = Value ? TVJumpVelocity : ManJumpVelocity;
	
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->JumpZVelocity = JumpVelocity;
	}	
}

void ABaseCharacter::SetLeaping(bool Value)
{
	UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->SetLeaping(Value);
}

void ABaseCharacter::SetVelocity(const float Value)
{
	UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();
	if (!CharacterMovementComp)
	{
		return;
	}

	FVector NewVelocity = CharacterMovementComp->Velocity.GetSafeNormal();
	NewVelocity *= Value;
	CharacterMovementComp->Velocity = NewVelocity;
}
