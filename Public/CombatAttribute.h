#pragma once
#include "CoreMinimal.h"
#include "CombatAttribute.Generated.h"

UENUM(BlueprintType)
enum class CombatAttributeName : uint8
{
	None				UMETA(DisplayName = "None"),
	CriticalHitChance	UMETA(DisplayName = "Critical Hit Chance"),
	CriticalHitDamage	UMETA(DisplayName = "Critical Hit Damage"),
	CooldownRate		UMETA(DisplayName = "Cooldown Rate"),
	MovementSpeed		UMETA(DisplayName = "Movement Speed"),
	Haste				UMETA(DisplayName = "Haste"),
	HealingDone			UMETA(DisplayName = "Healing Done"),
	HealingTaken		UMETA(DisplayName = "Healing Taken")
};

UENUM(BlueprintType, meta = (ScriptName = "CDamageEvent"))
enum class EGameDamageType : uint8
{
	All				UMETA(DisplayName = "All"),
	Physical			UMETA(Displayname = "Physical"),
	Fire				UMETA(Displayname = "Fire"),
	Frost				UMETA(Displayname = "Frost"),
	Void				UMETA(DisplayName = "Void"),
	Astral				UMETA(DisplayName = "Astral"),
	Lightning			UMETA(Displayname = "Lightning"),
	Holy				UMETA(Displayname = "Holy"),
	Essence				UMETA(DisplayName = "Essence"),
	Nature				UMETA(DisplayName = "Nature")
};


UENUM(BlueprintType)
enum class StatModifier : uint8
{
	Multiplicative	UMETA(Displayname = "Multiplicative"),
	Additive		UMETA(DisplayName = "Additive")
};

USTRUCT(BlueprintType)
struct HELENAPLAYGROUND_API FCombatAttribute
{
	GENERATED_USTRUCT_BODY()

protected:
	TMap<uint32, float> AdditiveBonuses;
	TMap<uint32, float> MultiplicativeBonuses;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float BaseValue = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
		float CurrentValue = BaseValue;

	FCombatAttribute() {};
	FCombatAttribute(const float Base) { BaseValue = Base; CurrentValue = Base; };

	void AddEffect(uint32 EffectID, StatModifier Modifier, float value)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ADD EFFECT"));
		switch (Modifier)
		{
		case StatModifier::Additive:
			AdditiveBonuses.Add(EffectID, value);
			break;
		case StatModifier::Multiplicative:
			MultiplicativeBonuses.Add(EffectID, value);
			break;
		}

		CurrentValue = GetFinalValue();
	}

	void RemoveEffect(uint32 EffectID, StatModifier Modifier)
	{
		switch (Modifier)
		{
		case StatModifier::Additive:
			AdditiveBonuses.Remove(EffectID);
			break;
		case StatModifier::Multiplicative:
			MultiplicativeBonuses.Remove(EffectID);
			break;
		}

		CurrentValue = GetFinalValue();
	}

	float GetFinalValue()
	{
		float AdditiveSum = 0;
		float MultiplicativeProduct = 1;

		for (auto& ModIt : AdditiveBonuses)
			AdditiveSum += ModIt.Value;

		for (auto& ModIt : MultiplicativeBonuses)
			MultiplicativeProduct *= ModIt.Value;

		return BaseValue * MultiplicativeProduct + AdditiveSum;
	}
};
