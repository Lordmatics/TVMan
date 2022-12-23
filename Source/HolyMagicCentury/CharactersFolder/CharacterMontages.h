// Lordmatics Games December 2022

#pragma once

#include "CoreMinimal.h"
#include "CharacterMontages.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FCharacterMontages
{
	GENERATED_BODY()

public:

	FCharacterMontages();	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* UnsheatheMontage;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* SheatheMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* LungeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* SlashMontage;
};