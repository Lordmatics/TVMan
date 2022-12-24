// Lordmatics Games December 2022

#include "HolyMagicCentury/CharactersFolder/BaseCharacter.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "../AnimationFolder/BaseCharacterAnimationInstance.h"
#include <Materials/MaterialInstanceDynamic.h>
#include "../ActionsFolder/ActionBase.h"
#include "../ActionsFolder/LeapAction.h"
#include "../ActionsFolder/ActionManager.h"
#include "../ActionsFolder/DefaultAction.h"
#include "../ActionsFolder/HideAction.h"
#include "../ActionsFolder/JumpAction.h"
#include "../ActionsFolder/GroundSlamAction.h"
#include <Engine/EngineTypes.h>
#include "../ActionsFolder/UnsheatheAction.h"
#include "../ActionsFolder/SheatheAction.h"
#include "../ActionsFolder/LungeAction.h"
#include "../ActionsFolder/SlashAction.h"
#include "../ActionsFolder/SpinningSlashAction.h"

// Sets default values
ABaseCharacter::ABaseCharacter() :
	InitialGravityScale(1.0f),
	InitialJumpVelocity(600.0f),
	bMovementDisabled(false),
	bRotationDisabled(false),
	WalkSpeed(300.0f),
	RunSpeed(600.0f),
	MontagePacket()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->InitCapsuleSize(42.0f, 96.0f);
	}

	CrouchDataPacket = FCrouchData(GetCapsuleComponent(), GetMesh());

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

	LeftAntenna = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LAntenna"));
	LeftAntenna->SetupAttachment(GetMesh(), TEXT("TV"));
	LeftAntenna->SetRelativeScale3D(FVector(0.05f));
	LeftAntenna->SetRelativeLocation(FVector(0.326331f, 0.0f, 0.978994f));
	LeftAntenna->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightAntenna = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RAntenna"));
	RightAntenna->SetupAttachment(GetMesh(), TEXT("TV"));
	RightAntenna->SetRelativeScale3D(FVector(0.05f));
	RightAntenna->SetRelativeLocation(FVector(-0.326331f, 0.0f, 0.978994f));
	RightAntenna->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	RightAntenna->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

ABaseCharacter::~ABaseCharacter()
{
	volatile int i = 5;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ActionManager = NewObject<UActionManager>(this);
	ActionManager->InitialiseManager(this);

	UDefaultActionData* ActionData = NewObject<UDefaultActionData>(this);
	ActionManager->RequestAction(ActionNames::DefaultAction, ActionData);

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

	if (RightAntenna)
	{
		const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		const FName SocketToAttach("RightAntennaSocket");
		RightAntenna->AttachToComponent(SkeletalMesh, Rules, SocketToAttach);
		RightAntenna->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (LeftAntenna)
	{
		const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		const FName SocketToAttach("LeftAntennaSocket");
		LeftAntenna->AttachToComponent(SkeletalMesh, Rules, SocketToAttach);
		LeftAntenna->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
		const float GravityScale = CharacterMovementComp->GravityScale;
		InitialGravityScale = GravityScale;

		const float JumpVel = CharacterMovementComp->JumpZVelocity;
		CharacterMovementComp->JumpZVelocity = JumpVel * GravityScale * 1.33f;
		InitialJumpVelocity = JumpVel * GravityScale * 1.33f;
	}	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		const FVector& Vel = CharacterMovementComponent->Velocity;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Vel: X: %.1f, Y: %.1f, Z: %.1f"), Vel.X, Vel.Y, Vel.Z));
		}
	}

	if (ActionManager)
	{
		// Return to most recent action ?
		const bool bProcessSuccess = ActionManager->Process(DeltaTime);
		if (!bProcessSuccess)
		{
			const bool bHasWeaponOut = IsWeaponUnsheathed();
			if (bHasWeaponOut)
			{
				const bool bHasEntries = !AttackBuffer.IsEmpty();
				if (bHasEntries)
				{
					FName Item;
					if (AttackBuffer.Dequeue(Item))
					{							
						if (Item == ActionNames::SlashAction)
						{
							USlashActionData* ActionData = NewObject<USlashActionData>(this);
							ActionManager->RequestAction(Item, ActionData);
							return;
						}	
						else if (Item == ActionNames::SpinningSlashAction)
						{						
							USpinningSlashActionData* ActionData = NewObject<USpinningSlashActionData>(this);
							ActionManager->RequestAction(ActionNames::SpinningSlashAction, ActionData);
							return;
						}
					}
				}
			}

			// TODO: See if i can streamline this later.
			const FName& LastKnownActionName = ActionManager->GetLastKnownDefaultAction();
			if (LastKnownActionName == ActionNames::DefaultAction)
			{
				UDefaultActionData* ActionData = NewObject<UDefaultActionData>(this);
				ActionManager->RequestAction(ActionNames::DefaultAction, ActionData);
			}
			else if (LastKnownActionName == ActionNames::HideAction)
			{
				UHideActionData* ActionData = NewObject<UHideActionData>(this);
				ActionManager->RequestAction(ActionNames::HideAction, ActionData);
			}
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("ActionName: %s"), *ActionManager->GetCurrentActionName()));
		}
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::OnAttackPressed);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABaseCharacter::OnAttackReleased);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABaseCharacter::StopJumping);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ABaseCharacter::OnActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ABaseCharacter::OnActionReleased);

	PlayerInputComponent->BindAction("North", IE_Pressed, this, &ABaseCharacter::OnNorthPressed);
	PlayerInputComponent->BindAction("North", IE_Released, this, &ABaseCharacter::OnNorthReleased);

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

	if (bMovementDisabled)
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
	if (bRotationDisabled)
	{
		return;
	}

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
	if (bRotationDisabled)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float DeltaSeconds = World->GetDeltaSeconds();
	const float PitchInput = Rate * BaseLookUpRate * DeltaSeconds;

	AddControllerPitchInput(PitchInput);
}

void ABaseCharacter::AddControllerYawInput(float Value)
{
	if (bRotationDisabled)
	{
		return;
	}

	APawn::AddControllerYawInput(Value);
}

void ABaseCharacter::AddControllerPitchInput(float Value)
{
	if (bRotationDisabled)
	{
		return;
	}

	Value *= -1.0f;
	APawn::AddControllerPitchInput(Value);
}

FName ABaseCharacter::GetLastKnownDefaultActionName() const
{
	if (ActionManager)
	{
		return ActionManager->GetLastKnownDefaultAction();
	}
	return NAME_None;
}

const float ABaseCharacter::GetWeaponStrength() const
{
	return 500.0f;
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

	if (ActionManager)
	{
		UJumpActionData* ActionData = NewObject<UJumpActionData>(this);
		ActionManager->RequestAction(ActionNames::JumpAction, ActionData);
	}

	ACharacter::Jump();
}

void ABaseCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void ABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (ActionManager)
	{
		ActionManager->OnLanded(Hit);

		if (ActionManager->IsCurrentAction(ActionNames::JumpAction))
		{
			UDefaultActionData* ActionData = NewObject<UDefaultActionData>(this);
			ActionManager->RequestAction(ActionNames::DefaultAction, ActionData);
		}		
	}
}

void ABaseCharacter::OnAttackPressed()
{
	if (!ActionManager)
	{
		return;
	}

	const bool bIsSpinning = ActionManager->IsCurrentAction(ActionNames::SpinningSlashAction);

	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		const bool bIsFalling = CharacterMovementComp->IsFalling();
		if (bIsFalling)
		{
			const bool bIsSlamming = ActionManager->IsCurrentAction(ActionNames::GroundSlamAction);
			if (!bIsSlamming)
			{
				const bool bHiding = ActionManager->IsCurrentAction(ActionNames::HideAction);
				UGroundSlamActionData* ActionData = NewObject<UGroundSlamActionData>(this);
				ActionManager->RequestAction(ActionNames::GroundSlamAction, ActionData);
			}
			return;
		}
	}

	const bool bHasWeaponOut = IsWeaponUnsheathed();
	if (bHasWeaponOut)
	{
		const bool bIsSlashing = ActionManager->IsCurrentAction(ActionNames::SlashAction);
		if (bIsSlashing)
		{
			if (AttackBuffer.IsEmpty())
			{
				AttackBuffer.Enqueue(ActionNames::SpinningSlashAction);
			}		
			return;
		}

		const bool bLunging = ActionManager->IsCurrentAction(ActionNames::LungeAction);
		if (!bLunging)
		{
			if (AttackBuffer.IsEmpty())
			{
				ULungeActionData* ActionData = NewObject<ULungeActionData>(this);
				ActionManager->RequestAction(ActionNames::LungeAction, ActionData);
			}
			else
			{
				// Buffer Slash.
				//AttackBuffer.Enqueue(ActionNames::LungeAction);
			}
		}
		else
		{
			if (AttackBuffer.IsEmpty())
			{
				AttackBuffer.Enqueue(ActionNames::SlashAction);
			}
		}
	}
}

void ABaseCharacter::OnAttackReleased()
{

}

void ABaseCharacter::OnNorthPressed()
{
	// Temp
	// Grab Antenna
	if (ActionManager)
	{
		const bool bHiding = ActionManager->IsCurrentAction(ActionNames::HideAction);
		if (!bHiding)
		{
			const bool bHasWeaponOut = IsWeaponUnsheathed();
			if (bHasWeaponOut)
			{
				USheatheActionData* ActionData = NewObject<USheatheActionData>(this);
				ActionManager->RequestAction(ActionNames::SheatheAction, ActionData);
				return;
			}
			else
			{
				UUnsheatheActionData* ActionData = NewObject<UUnsheatheActionData>(this);
				ActionManager->RequestAction(ActionNames::UnsheatheAction, ActionData);
				return;
			}
		}
	}
}

void ABaseCharacter::OnNorthReleased()
{

}

void ABaseCharacter::OnActionPressed()
{
	const bool bHasWeaponOut = IsWeaponUnsheathed();

	if (ActionManager)
	{
		// TODO: Make it so you cant instantly exit this.
		const bool bHiding = ActionManager->IsCurrentAction(ActionNames::HideAction);
		if (bHiding)
		{
			UDefaultActionData* ActionData = NewObject<UDefaultActionData>(this);
			ActionManager->RequestAction(ActionNames::DefaultAction, ActionData);
		}
		else if(!bHasWeaponOut)
		{
			UHideActionData* ActionData = NewObject<UHideActionData>(this);
			ActionManager->RequestAction(ActionNames::HideAction, ActionData);
		}
	}
}

void ABaseCharacter::OnActionReleased()
{

}

void ABaseCharacter::OnLeapPressed()
{
	if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
	{
		const bool bIsFalling = CharacterMovementComp->IsFalling();
		if (bIsFalling)
		{
			return;
		}
	}

	if (ActionManager)
	{
		const bool bAlreadyInLeap = ActionManager->GetCurrentActionName().Equals(ActionNames::LeapAction.ToString());
		if (bAlreadyInLeap)
		{
			return;
		}

		ULeapActionData* LeapData = NewObject<ULeapActionData>(this);
		ActionManager->RequestAction(ActionNames::LeapAction, LeapData);
	}
}

void ABaseCharacter::OnLeapReleased()
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

// Root Motion in the Z Only works if you are in flying mode!
void ABaseCharacter::SetFlying(bool Value)
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		if (Value)
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Flying);
		}
		else
		{			
			CharacterMovementComponent->SetDefaultMovementMode();
		}		
	}
}

void ABaseCharacter::EndAction()
{
	if (ActionManager)
	{
		ActionManager->EndCurrentAction();
	}
}

void ABaseCharacter::AttachAntennaToHand()
{
	if (!RightAntenna)
	{
		return;
	}

	USkeletalMeshComponent* BodyMesh = GetMesh();
	if (!BodyMesh)
	{
		return;
	}

	const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	RightAntenna->AttachToComponent(BodyMesh, Rules, SocketNames::WeaponSocket);
}

void ABaseCharacter::DetachAntennaFromHand()
{
	if (!RightAntenna)
	{
		return;
	}

	USkeletalMeshComponent* BodyMesh = GetMesh();
	if (!BodyMesh)
	{
		return;
	}

	const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	const FName SocketToAttach("RightAntennaSocket");
	RightAntenna->AttachToComponent(BodyMesh, Rules, SocketToAttach);

	//RightAntenna->SetupAttachment(BodyMesh, TEXT("TV"));
	//RightAntenna->SetRelativeScale3D(FVector(0.05f));
	//RightAntenna->SetRelativeLocation(FVector(-0.326331f, 0.0f, 0.978994f));
	//RightAntenna->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//RightAntenna->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABaseCharacter::IsWeaponUnsheathed() const
{
	bool bHasWeaponOut = false;
	if (UBaseCharacterAnimationInstance* AnimInstance = GetAnimInstance())
	{
		bHasWeaponOut = AnimInstance->IsAntennaUnsheathed();
	}
	return bHasWeaponOut;
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

bool ABaseCharacter::GetWeaponLocation(FVector& WeaponLocation) const
{
	if (USkeletalMeshComponent* SkeletalMesh = GetMesh())
	{
		WeaponLocation = SkeletalMesh->GetSocketLocation(SocketNames::WeaponSocket);
		return true;
	}
	return false;
}
