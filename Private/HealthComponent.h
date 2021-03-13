// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_Character.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UEffectAbsorbDamage;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthDelegate, AC_Character*, DamageDealer, float, IncomingDamage);

UCLASS( ClassGroup=(Custom), hidecategories = Object, config = Engine, editinlinenew, meta = (BlueprintSpawnableComponent))
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	AC_Character* Pawn;
	
	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere, Category = Health)
		float MaxHealth;

	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere, Category = Health)
		float CurrentHealth;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Health")
		FHealthDelegate OnDamageReceived;

	UPROPERTY(BlueprintReadWrite, Category = "Absorbs")
		TArray<UEffectAbsorbDamage*> Absorbs;

	UFUNCTION(BlueprintCallable)
		float GetDamageFactorForType(TEnumAsByte<DamageType> Type);

	UFUNCTION(BlueprintCallable)
	float DamageAfterCritCalculation(AC_Character* DamageDealer, const float IncomingDamage, const float IncreasedCriticalChance,
	                                 const float IncreasedCriticalDamage);

	UFUNCTION(BlueprintCallable)
	bool CheckCriticalHit(AC_Character* DamageDealer, const float IncreasedCriticalChance);

	UFUNCTION(BlueprintCallable)
	float CalculateDamageReduction(TEnumAsByte<DamageType> Type, float IncomingDamage);
	void CheckForAbsorbs(float IncomingDamage, float& AbsorbedDamage, float& NotAbsorbedDamage);
};
