// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleObject.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UDestructibleMesh;
class UDestructibleComponent;

UCLASS()
class HOLYMAGICCENTURY_API ADestructibleObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleObject();

	virtual void Tick(float DeltaTime) override;

	void Explode(const FVector& HitLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Destruction, meta = (ExposeFunctionCategories = "Destruction,Components|Destructible", AllowPrivateAccess = "true"))
	UDestructibleComponent* DestructibleComponent;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	float ImpulseStrength;

};
