#pragma once

#include "CombatAttribute.h"
#include "C_Character.h"
#include "EffectBase.h"
#include "EffectAttributeModifier.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMPYREAN_API UEffectAttributeModifier : public UEffectBase
{

	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		CombatAttributeName CombatAttributeToModify = CombatAttributeName::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		StatModifier Modifier = StatModifier::Multiplicative;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
		float Amount = 0;

	void Initialize_Implementation(UStatusBase* ParentStatus) override;
	
	void OnExpired_Implementation() override;
};

inline void UEffectAttributeModifier::Initialize_Implementation(UStatusBase* ParentStatus)
{
	Super::Initialize_Implementation(ParentStatus);
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		Pawn->CombatAttributes[CombatAttributeToModify].AddEffect(GetUniqueID(), Modifier, Amount);
	}
}

inline void UEffectAttributeModifier::OnExpired_Implementation()
{
	AC_Character* Pawn = Status->TargetActor;
	if (Pawn)
	{
		Pawn->CombatAttributes[CombatAttributeToModify].RemoveEffect(GetUniqueID(), Modifier);
	}
}


