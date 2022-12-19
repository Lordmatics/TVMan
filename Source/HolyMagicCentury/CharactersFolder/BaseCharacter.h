// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UObject/NoExportTypes.h>
#include "HolyMagicCentury/CharactersFolder/CrouchData.h"
#include "BaseCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UBaseCharacterAnimationInstance;
class UMaterialInstanceDynamic;
class UActionBase;
class UActionManager;

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
	FORCEINLINE const FCrouchData& GetCrouchData() const { return CrouchDataPacket; }
	FORCEINLINE FCrouchData& GetCrouchData() { return CrouchDataPacket; }

	virtual void Jump() override;	
	virtual void StopJumping() override;

	virtual void Landed(const FHitResult& Hit) override;

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
		TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		UActionManager* ActionManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
		float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
		FCrouchData CrouchDataPacket;

};
