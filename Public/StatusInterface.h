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
	UStatusBase* AddStatus(UStatusBase* Status);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Status")
		void IRemoveStatus(UStatusBase* Status);
};
