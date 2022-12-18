// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UObject/NoExportTypes.h>
#include "BaseCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UBaseCharacterAnimationInstance;
class UMaterialInstanceDynamic;

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

	FORCEINLINE USpringArmComponent* GetSpringArm() const { return ThirdPersonSpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return ThirdPersonCamera; }

	virtual void Jump() override;	
	virtual void StopJumping() override;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnActionPressed();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void OnActionReleased();

	void SetBodyAlpha(float Value);
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
	
	UBaseCharacterAnimationInstance* GetAnimInstance() const;
	void OnMovementRequested(float Value);
	void SetWalking(bool Value);
	void SetRunning(bool Value);
	void SetHiding(bool Value);
	void ResetAnimationMovement();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* ThirdPersonSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

};