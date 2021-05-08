// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDamageEvent.h"
#include "Components/ActorComponent.h"
#include "AbilityBase.generated.h"

class UEffectBase;
class AC_Character;
class UStatusBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDealtDamage, AC_Character*, Target, float, Damage, UAbilityBase*, Ability, bool, Periodic);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealedUnit, AC_Character*, Target, float, Healing, UAbilityBase*, Ability, bool, Periodic);

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELENAPLAYGROUND_API UAbilityBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void ConeTrace(FVector ConeOrigin, bool TargetFriendly, bool TargetEnemy, bool IgnoreSelf, float Range, float ConeAngle, bool DrawDebug,
			TArray<AC_Character*>& CharactersHit);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FOnDealtDamage OnDealtDamage;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FOnHealedUnit OnHealedUnit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTrace;


	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = RemoveResource))
	void BPRemoveResource();
	virtual void BPRemoveResource_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = GenerateResource))
	void BPGenerateResource();
	virtual void BPGenerateResource_Implementation() {}

	UFUNCTION(BlueprintCallable, meta = (DeterminesoutputType = "AbilityClass"), BlueprintPure)
	UAbilityBase* LookForAbility(const TSubclassOf<UAbilityBase> AbilityClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAbilityBase* GetPassive();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastGenerateResource();
	void MulticastGenerateResource_Implementation() { BPGenerateResource(); }

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastRemoveResource();
	void MulticastRemoveResource_Implementation() { BPRemoveResource(); }

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadwrite, SimpleDisplay, Category = "Status", Meta = (DisplayName = "Status", ExposeFunctionCategories="Status", AllowPrivateAccess="true"))
		TArray<UStatusBase*> StatusToApply;

	UFUNCTION(BlueprintCallable)
		TArray<AC_Character*> GetTargetsInRadius(const FVector Center, const float Radius, const bool IgnoreSelf, const bool Friendly, 
			const bool Enemy, const bool DrawDebug);
	
	UFUNCTION(BlueprintCallable)
		void DealDamage(AC_Character* Target, FCharacterDamageEvent Event, float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool&
		                IsKillingBlow);

	UFUNCTION(BlueprintCallable)
		UStatusBase* ApplyStatus(AC_Character* Target, int StatusIndex, bool& Refreshed, bool OverrideDuration = false, float Duration = 0.f);

	UFUNCTION(BlueprintCallable)
		void LookForStatus(AC_Character* Target, bool IsDebuff, TSubclassOf<UStatusBase> StatusToLookFor, UStatusBase*& FoundStatus, int& Stacks, bool OwnOnly = true);

	UFUNCTION(BlueprintCallable)
		void HealUnit(AC_Character* Target, FCharacterDamageEvent Event);
	
	UFUNCTION(BlueprintNativeEvent)
	void Initialize(AActor* InCaster, int AbilitySlot);
	virtual void Initialize_Implementation(AActor* InCaster, int AbilitySlot) {};

	UFUNCTION()
	void CastAbility() {};

	UPROPERTY(BlueprintReadOnly)
		AC_Character* Caster;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EGameDamageType AbilitySchool;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName AbilityName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* Icon;
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	FCombatAttribute CastTime;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	bool CastableWhileMoving;
	//
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	bool CastableWhileCasting;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//	bool IsChanneled;

	
};
