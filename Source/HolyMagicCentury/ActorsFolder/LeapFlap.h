// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeapFlap.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAnimMontage;

UCLASS()
class HOLYMAGICCENTURY_API ALeapFlap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeapFlap();

	virtual void Tick(float DeltaTime) override;

	void OpenFront();

	void OpenBack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	bool IsMontagePlaying() const;
	bool PlayAnimMontage(UAnimMontage* Montage);

	UFUNCTION()
	void RestoreCollision();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeapFlapRim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LeapFlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* OpenFlapFrontAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* OpenFlapBackAnimMontage;

	FTimerHandle RestoreCollisionHandle;	
};
