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
		EGameDamageType ResistanceToModify = EGameDamageType::All;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		StatModifier Modifier = StatModifier::Multiplicative;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		float Amount = 0;

	void Initialize_Implementation(UStatusBase* ParentStatus) override;

	void OnExpired_Implementation() override;
};

inline void UEffectDamageResistanceModifier::Initialize_Implementation(UStatusBase* ParentStatus)
{
	Super::Initialize_Implementation(ParentStatus);
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		Pawn->DamageResistance[ResistanceToModify].AddEffect(GetUniqueID(), Modifier, Amount);
	}
}

inline void UEffectDamageResistanceModifier::OnExpired_Implementation()
{
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		Pawn->DamageResistance[ResistanceToModify].RemoveEffect(GetUniqueID(), Modifier);
	}
}