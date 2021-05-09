#pragma once

#include "CombatAttribute.h"
#include "C_Character.h"
#include "EffectBase.h"
#include "HealthComponent.h"
#include "EffectMaxHealthModifier.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMPYREAN_API UEffectMaxHealthModifier : public UEffectBase
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		StatModifier Modifier = StatModifier::Multiplicative;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		float Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		bool HealableOnly;

	void Initialize_Implementation(UStatusBase* ParentStatus) override;

	void OnExpired_Implementation() override;
};

inline void UEffectMaxHealthModifier::Initialize_Implementation(UStatusBase* ParentStatus)
{
	Super::Initialize_Implementation(ParentStatus);
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		FCombatAttribute Health = HealableOnly ? Pawn->Health->MaxHealableHealth : Pawn->Health->MaxHealth;
		Health.AddEffect(GetUniqueID(), Modifier, Amount);
	}
}

inline void UEffectMaxHealthModifier::OnExpired_Implementation()
{
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		FCombatAttribute Health = HealableOnly ? Pawn->Health->MaxHealableHealth : Pawn->Health->MaxHealth;
		Health.RemoveEffect(GetUniqueID(), Modifier);
	}
}