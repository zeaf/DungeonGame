// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectBase.h"
#include "EffectPeriodic.generated.h"

/**
 * 
 */
UCLASS()
class HELENAPLAYGROUND_API UEffectPeriodic : public UEffectBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		FTimerHandle TimerHandle;

	UPROPERTY()
		float Duration;

	UPROPERTY()
		int TicksDone = 0;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		int32 NumberOfTicks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		float TickInterval = 0;
	
		void Initialize(UStatusBase* ParentStatus) override;

		void OnRemoved() override;
	
		void OnExpired() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnTick();

	virtual void OnTick_Implementation();

};