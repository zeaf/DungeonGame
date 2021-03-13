#pragma once

#include "CombatAttribute.h"
#include "C_Character.h"
#include "EffectBase.h"
#include "EffectDamageFactorModifier.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELENAPLAYGROUND_API UEffectDamageFactorModifier : public UEffectBase
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		DamageType DamageFactorToModify = DamageType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		StatModifier Modifier = StatModifier::Multiplicative;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		float Amount = 0;

	void Initialize() override;
};

inline void UEffectDamageFactorModifier::Initialize()
{
	AC_Character* Pawn = Cast<AC_Character>(GetOuter());
	if (Pawn)
	{
		Pawn->DamageFactors[DamageFactorToModify].AddEffect(GetUniqueID(), Modifier, Amount);
	}
}