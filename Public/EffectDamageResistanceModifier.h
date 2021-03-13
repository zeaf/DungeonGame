#pragma once

#include "CombatAttribute.h"
#include "C_Character.h"
#include "EffectBase.h"
#include "EffectDamageResistanceModifier.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELENAPLAYGROUND_API UEffectDamageResistanceModifier : public UEffectBase
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		DamageType ResistanceToModify = DamageType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		StatModifier Modifier = StatModifier::Multiplicative;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		float Amount = 0;

	void Initialize() override;

	void OnRemoved() override;
};

inline void UEffectDamageResistanceModifier::Initialize()
{
	AC_Character* Pawn = Cast<AC_Character>(GetOuter());
	if (Pawn)
	{
		Pawn->DamageResistance[ResistanceToModify].AddEffect(GetUniqueID(), Modifier, Amount);
	}
}

inline void UEffectDamageResistanceModifier::OnRemoved()
{
	AC_Character* Pawn = Cast<AC_Character>(GetOuter());
	if (Pawn)
	{
		Pawn->DamageResistance[ResistanceToModify].RemoveEffect(GetUniqueID(), Modifier);
	}
}