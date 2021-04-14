#pragma once

#include "CoreMinimal.h"

#include "StatusBase.h"

#include "StatusInterface.generated.h"


UINTERFACE(Blueprintable)
class UStatusInterface : public UInterface
{
	GENERATED_BODY()
};

class IStatusInterface
{
	GENERATED_BODY()

public:
		//class UStatusBase;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Status")
	UStatusBase* AddStatus(UStatusBase* Status, class AC_Character* Caster, class UAbilityBase* Ability, bool& Refreshed);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Status")
		void IRemoveStatus(UStatusBase* Status);
};
