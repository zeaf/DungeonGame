#include "EffectAbsorbDamage.h"
#include "C_Character.h"
#include "HealthComponent.h"

inline void UEffectAbsorbDamage::Initialize(UStatusBase* ParentStatus)
{
	Super::Initialize(ParentStatus);
	if (Status->TargetActor)
		Status->TargetActor->Health->Absorbs.Add(this);
}

inline void UEffectAbsorbDamage::OnRemoved()
{
	if (Status->TargetActor)
		Status->TargetActor->Health->Absorbs.Remove(this);
}