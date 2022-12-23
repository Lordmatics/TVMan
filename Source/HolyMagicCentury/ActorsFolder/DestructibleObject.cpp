// Lordmatics Games December 2022


#include "HolyMagicCentury/ActorsFolder/DestructibleObject.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <../Plugins/Runtime/ApexDestruction/Source/ApexDestruction/Public/DestructibleMesh.h>
#include <../Plugins/Runtime/ApexDestruction/Source/ApexDestruction/Public/DestructibleComponent.h>

// Sets default values
ADestructibleObject::ADestructibleObject() :
	ImpulseStrength(50000.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(Root);

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Mesh"));
	DestructibleComponent->SetupAttachment(Root);

	//SDestrucibleMesh = CreateDefaultSubobject<UDestructibleMesh>(TEXT("Destructible Mesh"));S	
}

// Called when the game starts or when spawned
void ADestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructibleObject::Explode(const FVector& HitLocation, const FVector& ImpactNormal, const float ImpactStrength)
{	
	if (DestructibleComponent)
	{
		if (StaticMesh)
		{
			StaticMesh->SetVisibility(false);
			StaticMesh->DestroyComponent();
		}//FVector::DownVector

		const float Strength = ImpactStrength == 0.0f ? ImpulseStrength : ImpactStrength;
		DestructibleComponent->ApplyDamage(1000.0f, HitLocation, -ImpactNormal, Strength);
		DestructibleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

