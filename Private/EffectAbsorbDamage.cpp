#include "EffectAbsorbDamage.h"
#include "C_Character.h"
#include "HealthComponent.h"

inline void UEffectAbsorbDamage::Initialize_Implementation(UStatusBase* ParentStatus)
{
	Super::Initialize_Implementation(ParentStatus);
	if (Status->TargetActor)
		Status->TargetActor->Health->Absorbs.Add(this);
}

inline void UEffectAbsorbDamage::OnExpired_Implementation()
{
	if (Status->TargetActor)
		Status->TargetActor->Health->Absorbs.Remove(this);
}