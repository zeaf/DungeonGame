// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StatusBase.h"
#include "UObject/NoExportTypes.h"
#include "EffectBase.generated.h"

class UStatusBase;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMPYREAN_API UEffectBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		UStatusBase* Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SelfOnly;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Initialize(UStatusBase* ParentStatus);

	virtual void Initialize_Implementation(UStatusBase* ParentStatus)
	{
		Status = ParentStatus;
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnRemoved();
	virtual void OnRemoved_Implementation() { OnExpired(); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnExpired();
	virtual void OnExpired_Implementation() { }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnRefreshed();
	virtual void OnRefreshed_Implementation() { }
};
