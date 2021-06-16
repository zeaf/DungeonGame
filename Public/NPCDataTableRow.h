#pragma once
#include "Engine/DataTable.h"

#include "C_Character.h"
#include "ActiveAbilityBase.h"
#include "NPCDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct EMPYREAN_API FNPCCharacterDataTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere)
		TSubclassOf<AC_Character> Character;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<UActiveAbilityBase>> ActiveAbilities;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<UAbilityBase>> PassiveAbilities;
	UPROPERTY(EditAnywhere)
		float AutoAttackDamage;
	UPROPERTY(EditAnywhere)
		float AutoAttackCD = 1.5f;
	UPROPERTY(EditAnywhere)
		float AutoAttackRange = 220;
	UPROPERTY(EditAnywhere)
		float MaxHealth;
	UPROPERTY(EditAnywhere)
		float Minhealth;
};
