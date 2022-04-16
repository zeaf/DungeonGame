// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatEvents.h"
#include "EffectPeriodic.h"
#include "EffectDamageOverTime.generated.h"

/**
 * 
 */
UCLASS()
class EMPYREAN_API UEffectDamageOverTime : public UEffectPeriodic
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float TotalDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float AdditionalCriticalChance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float AdditionalCriticalDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		EGameDamageType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool KeepPreviousDamage;
	
protected:
	float DamagePerTick;

	FCharacterDamageEvent Event;

public:
	void Initialize_Implementation(UStatusBase* ParentStatus) override;

	void OnTick_Implementation() override;

	virtual void OnRefreshed_Implementation() override;
};
