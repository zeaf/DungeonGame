// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDamageEvent.h"
#include "Components/ActorComponent.h"
#include "AbilityBase.generated.h"

class AC_Character;
class UStatusBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDealtDamage, AC_Character*, Target, float, Damage, UAbilityBase*, Ability);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealedUnit, AC_Character*, Target, float, Healing, UAbilityBase*, Ability);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELENAPLAYGROUND_API UAbilityBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadwrite, SimpleDisplay, Category = "Status", Meta = (DisplayName = "Status", ExposeFunctionCategories="Status", AllowPrivateAccess="true"))
		TArray<UStatusBase*> StatusToApply;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FOnDealtDamage OnDealtDamage;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FOnHealedUnit OnHealedUnit;

	UFUNCTION(BlueprintCallable)
		void DealDamage(AC_Character* Target, FCharacterDamageEvent Event);

	UFUNCTION(BlueprintCallable)
		void HealUnit(AC_Character* Target, FCharacterDamageEvent Event);
	
	UFUNCTION(BlueprintNativeEvent)
	void Initialize(AActor* Caster, int AbilitySlot);
	virtual void Initialize_Implementation(AActor* Caster, int AbilitySlot) {};

	UFUNCTION(BlueprintCallable)
	void ConeTrace(FVector ConeOrigin, bool TargetFriendly, bool TargetEnemy, bool IgnoreSelf, float Range, float ConeAngle, bool DrawDebug,
		TArray<AC_Character*>& CharactersHit);

	UFUNCTION()
	void CastAbility() {};

};
