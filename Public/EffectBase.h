// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectBase.generated.h"

class UStatusBase;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELENAPLAYGROUND_API UEffectBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
		UStatusBase* Status;
	
	UFUNCTION(BlueprintCallable)
		virtual void Initialize(UStatusBase* ParentStatus) { Status = ParentStatus; };

	UFUNCTION(BlueprintCallable)
	virtual void OnRemoved() {};
};
