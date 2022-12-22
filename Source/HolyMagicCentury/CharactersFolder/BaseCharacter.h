// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UObject/NoExportTypes.h>
#include "HolyMagicCentury/CharactersFolder/CrouchData.h"
#include "CharacterMontages.h"
#include "BaseCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UBaseCharacterAnimationInstance;
class UMaterialInstanceDynamic;
class UActionBase;
class UActionManager;
class UAnimMontage;
class UStaticMeshComponent;

namespace MaterialParameterNames
{
	const FName Alpha = FName(TEXT("Alpha"));
}

UCLASS()
class HOLYMAGICCENTURY_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual ~ABaseCharacter();

	FORCEINLINE USpringArmComponent* GetSpringArm() const { return ThirdPersonSpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return ThirdPersonCamera; }
	FORCEINLINE const FCrouchData& GetCrouchData() const { return CrouchDataPacket; }
	FORCEINLINE FCrouchData& GetCrouchData() { return CrouchDataPacket; }
	FORCEINLINE float GetDefaultGravity() const { return InitialGravityScale	; }
	FORCEINLINE float GetDefaultJumpVelocity() const { return InitialJumpVelocity; }
	FORCEINLINE float IsMovementDisabled() const { return bMovementDisabled; }
	FORCEINLINE float IsRotationDisabled() const { return bRotationDisabled; }
	FORCEINLINE void SetMovementDisabled(bool Value) { bMovementDisabled = Value; }
	FORCEINLINE void SetRotationDisabled(bool Value) { bRotationDisabled = Value; }
	FName GetLastKnownDefaultActionName() const;
	FORCEINLINE const FCharacterMontages& GetMontagePacket() const { return MontagePacket; }
	FORCEINLINE FCharacterMontages& GetMontagePacket() { return MontagePacket; }

	virtual void Jump() override;	
	virtual void StopJumping() override;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable, Category = "Character")
		void OnAttackPressed();

	UFUNCTION(BlueprintCallable, Category = "Character")
		void OnAttackReleased();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnNorthPressed();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnNorthReleased();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnActionPressed();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnActionReleased();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnLeapPressed();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnLeapReleased();

	void SetBodyAlpha(float Value);

	UBaseCharacterAnimationInstance* GetAnimInstance() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetFlying(bool Value);

	void EndAction();

	void AttachAntennaToHand();
	void DetachAntennaFromHand();

	bool IsWeaponUnsheathed() const;

private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveInDirection(EAxis::Type Axis, const float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	virtual void AddControllerYawInput(float Value) override;	
	virtual void AddControllerPitchInput(float Value) override;
	
	void SetVelocity(const float Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* ThirdPersonSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* LeftAntenna;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* RightAntenna;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		UActionManager* ActionManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float InitialGravityScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float InitialJumpVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool bMovementDisabled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		bool bRotationDisabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
		FCrouchData CrouchDataPacket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
		FCharacterMontages MontagePacket;

};
