#pragma once
#include "CoreMinimal.h"
#include "CharacterDamageEvent.Generated.h"

enum class DamageType : uint8;
class UAbilityBase;
class AC_Character;
USTRUCT(BlueprintType)
struct HELENAPLAYGROUND_API FCharacterDamageEvent
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	AC_Character* Instigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	UAbilityBase* Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	float AdditionalCriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	float AdditionalCriticalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	TEnumAsByte<DamageType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	bool IsPeriodic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	bool ApplyVariance;	
};