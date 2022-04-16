#pragma once

#include "CoreMinimal.h"
#include "CombatAttribute.h"
#include "CombatEvents.Generated.h"

//enum class EGameDamageType : uint8;
class UAbilityBase;
class AC_Character;
USTRUCT(BlueprintType)
struct EMPYREAN_API FCharacterDamageEvent
{
	GENERATED_BODY()

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
	EGameDamageType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	bool IsPeriodic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Event")
	bool ApplyVariance = true;	
};