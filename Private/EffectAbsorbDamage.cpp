#include "EffectAbsorbDamage.h"
#include "C_Character.h"
#include "HealthComponent.h"

inline void UEffectAbsorbDamage::Initialize(UStatusBase* ParentStatus)
{
	Super::Initialize(ParentStatus);
	AC_Character* Pawn = Cast<AC_Character>(GetOuter());

	if (Pawn)
		Pawn->Health->Absorbs.Add(this);
}

inline void UEffectAbsorbDamage::OnRemoved()
{
	AC_Character* Pawn = Cast<AC_Character>(GetOuter());

	if (Pawn)
		Pawn->Health->Absorbs.Remove(this);
}