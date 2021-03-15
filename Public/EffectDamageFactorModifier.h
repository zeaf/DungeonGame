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
		EGameDamageType DamageFactorToModify = EGameDamageType::All;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		StatModifier Modifier = StatModifier::Multiplicative;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		float Amount = 0;

	void Initialize(UStatusBase* ParentStatus) override;

	void OnExpired() override;
};

inline void UEffectDamageFactorModifier::OnExpired()
{
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		Pawn->DamageFactors[DamageFactorToModify].RemoveEffect(GetUniqueID(), Modifier);
	}
}

inline void UEffectDamageFactorModifier::Initialize(UStatusBase* ParentStatus)
{
	Super::Initialize(ParentStatus);
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		Pawn->DamageFactors[DamageFactorToModify].AddEffect(GetUniqueID(), Modifier, Amount);
	}
}