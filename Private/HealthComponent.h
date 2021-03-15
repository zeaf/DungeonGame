// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_Character.h"
#include "Components/ActorComponent.h"
#include "CharacterDamageEvent.h"
#include "HealthComponent.generated.h"

class UEffectAbsorbDamage;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageEventDelegate, AC_Character*, DamageDealer, float, IncomingDamage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealingEventDelegate, AC_Character*, Healer, float, IncomingHealing);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateHealthDelegate);

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

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Health")
		FUpdateHealthDelegate UpdateHealth;

	UFUNCTION()
	void OnRepCurrentHealth() const { UpdateHealth.Broadcast();}
	UFUNCTION()
	void OnRepMaxHealth() const { UpdateHealth.Broadcast(); }
	
	UPROPERTY()
	AC_Character* Pawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Health")
		float MinHealth;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Health")
		float MaxHealableHealth;
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRepMaxHealth, EditAnywhere, Category = "Health")
		float MaxHealth;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRepCurrentHealth, EditAnywhere, Category = "Health")
		float CurrentHealth;
	
	
	UPROPERTY(BlueprintReadWrite, Category = "Absorbs")
		TArray<UEffectAbsorbDamage*> Absorbs;


	UFUNCTION(BlueprintCallable)
	float GetCritMultiplier(AC_Character* DamageDealer, const float IncreasedCriticalChance,
	                        const float IncreasedCriticalDamage, bool& IsCrit);

	UFUNCTION(BlueprintCallable)
	bool CheckCriticalHit(AC_Character* DamageDealer, const float IncreasedCriticalChance);

	

	UFUNCTION(BlueprintCallable)
	void OnHit(FCharacterDamageEvent DamageEvent, float& FinalDamageTaken, float& DamageAbsorbed, bool& IsCrit, bool& IsKillingBlow);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region Damage
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Health")
		FDamageEventDelegate OnDamageReceived;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastReduceHealth(float IncomingDamage);

	virtual void MulticastReduceHealth_Implementation(float IncomingDamage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool DamageCharacter(float IncomingDamage);

	virtual bool DamageCharacter_Implementation(float IncomingDamage);

	UFUNCTION(BlueprintCallable)
		float GetDamageFactorForType(AC_Character* DamageDealer, EGameDamageType Type);
	
	UFUNCTION(BlueprintCallable)
	float CalculateDamageReduction(EGameDamageType Type);

	void CheckForAbsorbs(float IncomingDamage, float& AbsorbedDamage, float& NotAbsorbedDamage);
	
#pragma endregion

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Health")
		FHealingEventDelegate OnHealingReceived;
	
	UFUNCTION(NetMulticast, Reliable)
		void MulticastRestoreHealth(float IncomingHealing);

	virtual void MulticastRestoreHealth_Implementation(float IncomingHealing);

	UFUNCTION(BlueprintCallable)
		void OnHealReceived(FCharacterDamageEvent HealingEvent, float& FinalHealingTaken, bool& IsCrit);

	float GetHealingMultiplier(AC_Character* Healer);
	
};
