#pragma once

#include "EffectBase.h"
#include "EffectAbsorbDamage.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMPYREAN_API UEffectAbsorbDamage : public UEffectBase
{

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Absorb")
		float AbsorbAmount;


	void Initialize_Implementation(UStatusBase* ParentStatus) override;

	void OnExpired_Implementation() override;
};
