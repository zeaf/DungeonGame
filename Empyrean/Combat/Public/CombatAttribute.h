#pragma once
#include "CoreMinimal.h"
#include "CombatAttribute.Generated.h"

UENUM(BlueprintType)
enum class ECombatAttributeName : uint8
{
	None,
	CriticalHitChance	UMETA(DisplayName = "Critical Hit Chance"),
	CriticalHitDamage	UMETA(DisplayName = "Critical Hit Damage"),
	CooldownRate		UMETA(DisplayName = "Cooldown Rate"),
	MovementSpeed		UMETA(DisplayName = "Movement Speed"),
	Haste				UMETA(DisplayName = "Haste"),
	HealingDone			UMETA(DisplayName = "Healing Done"),
	HealingTaken		UMETA(DisplayName = "Healing Taken"),
	DO_NOT_DELETE		UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ECombatAttributeName, ECombatAttributeName::DO_NOT_DELETE)

UENUM(BlueprintType, meta = (ScriptName = "CDamageEvent"))
enum class EGameDamageType : uint8
{
	All					UMETA(DisplayName = "All"),
	Physical			UMETA(Displayname = "Physical"),
	Fire				UMETA(Displayname = "Fire"),
	Frost				UMETA(Displayname = "Frost"),
	Void				UMETA(DisplayName = "Void"),
	Astral				UMETA(DisplayName = "Astral"),
	Lightning			UMETA(Displayname = "Lightning"),
	Holy				UMETA(Displayname = "Holy"),
	Essence				UMETA(DisplayName = "Essence"),
	Nature				UMETA(DisplayName = "Nature"),
	DO_NOT_DELETE		UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EGameDamageType, EGameDamageType::DO_NOT_DELETE)


UENUM(BlueprintType)
enum class StatModifier : uint8
{
	Multiplicative	UMETA(Displayname = "Multiplicative"),
	Additive		UMETA(DisplayName = "Additive")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeValueChanged, float, NewValue);

USTRUCT(BlueprintType)
struct EMPYREAN_API FCombatAttribute
{
	GENERATED_USTRUCT_BODY()
	
protected:
	TMap<uint32, float> AdditiveBonuses;
	TMap<uint32, float> MultiplicativeBonuses;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
		float BaseValue = 1.f;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
		float CurrentValue = BaseValue;

	// Do not call this for gameplay reasons
	void SetValue(const float NewValue)
	{
		BaseValue = NewValue;
		CurrentValue = GetFinalValue();
	}
	
	FCombatAttribute() { GetFinalValue(); };
	FCombatAttribute(const float Base) { BaseValue = Base; CurrentValue = Base; };

	FAttributeValueChanged OnValueChange;
	
	void AddEffect(uint32 EffectID, StatModifier Modifier, float value)
	{
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

		float NewValue = BaseValue * MultiplicativeProduct + AdditiveSum;

		OnValueChange.Broadcast(NewValue);

		CurrentValue = NewValue;
		return NewValue;
	}
};

namespace CombatAttribute
{
	/**
	 * Creates a map with all the values of Enum E as keys, and FCombatAttribute as values
	 * @tparam E The enum used for the keys
	 * @return The created map
	 */
	template <typename E>
	static TMap<E, FCombatAttribute> CreateCombatAttributeMap()
	{
		TMap<E, FCombatAttribute> Temp;
		for (E Type : TEnumRange<E>())
		{
			FCombatAttribute Attribute;
			Temp.Add(Type, Attribute);
			Attribute.GetFinalValue();
		}
		return Temp;
	}
}