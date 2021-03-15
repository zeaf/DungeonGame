// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterDamageEvent.h"
#include "EffectPeriodic.h"
#include "EffectDamageOverTime.generated.h"

/**
 * 
 */
UCLASS()
class HELENAPLAYGROUND_API UEffectDamageOverTime : public UEffectPeriodic
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
		float TotalHealing = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
		float AdditionalCriticalChance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
		float AdditionalCriticalDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
		EGameDamageType Type;

protected:
	float HealingPerTick;

	FCharacterDamageEvent Event;

public:
	void Initialize(UStatusBase* ParentStatus) override;

	void OnTick_Implementation() override;
};
