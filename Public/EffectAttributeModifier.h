#pragma once

#include "CombatAttribute.h"
#include "C_Character.h"
#include "EffectBase.h"
#include "EffectAttributeModifier.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELENAPLAYGROUND_API UEffectAttributeModifier : public UEffectBase
{

	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		CombatAttributeName CombatAttributeToModify = CombatAttributeName::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		StatModifier Modifier = StatModifier::Multiplicative;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		float Amount = 0;

	void Initialize() override;

	void OnRemoved() override;
};

inline void UEffectAttributeModifier::Initialize()
{
	AC_Character* Pawn = Cast<AC_Character>(GetOuter());
	if (Pawn)
	{
		Pawn->CombatAttributes[CombatAttributeToModify].AddEffect(GetUniqueID(), Modifier, Amount);
	}
}

inline void UEffectAttributeModifier::OnRemoved()
{
	AC_Character* Pawn = Cast<AC_Character>(GetOuter());
	if (Pawn)
	{
		Pawn->CombatAttributes[CombatAttributeToModify].RemoveEffect(GetUniqueID(), Modifier);
	}
}

