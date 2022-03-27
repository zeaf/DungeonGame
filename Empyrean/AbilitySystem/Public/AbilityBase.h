// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDamageEvent.h"
#include "C_Character.h"
#include "Components/ActorComponent.h"
#include "AbilityBase.generated.h"

class UEffectBase;
class AC_Character;
class UStatusBase;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETargetingMask : uint8
{
	None			= 0 UMETA(Hidden),
	TargetFriendly	= 1 << 0,
	TargetEnemy		= 1 << 1,
	TargetSelf		= 1 << 2
};
ENUM_CLASS_FLAGS(ETargetingMask);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FOnDealtDamage, AC_Character*, Target, FCharacterDamageEvent, Event, float, TotalDamage, 
	bool, IsCrit, bool, IsKillingBlow, float, UnmitigatedDamage, float, DamageWithoutModifiers, float, DamageWithNoIncreases);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealedUnit, AC_Character*, Target, FCharacterDamageEvent, Event, float, TotalHealing, bool, IsCrit);

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMPYREAN_API UAbilityBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UAbilityBase();

protected:
	/**
	 * Perform a cone trace for targets
	 * @param ConeOrigin The origin of the cone
	 * @param ForwardVector The direction of the cone
	 * @param TargetsAllowed Mask allowed targets
	 * @param Range The length of the cone
	 * @param ConeAngle The angle of the cone trace
	 * @param DrawDebug Enable debug visualization
	 * @param CharactersHit The targets hit by the trace
	 * @param TraceChannel The channel to use to trace
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ConeTrace(const FVector ConeOrigin, const FVector ForwardVector, TArray<AC_Character*>& CharactersHit, 
		UPARAM(meta = (Bitmask, BitmaskEnum = "ETargetingMask")) int32 TargetsAllowed = 2,
		const float Range = 500, const float ConeAngle = 30, const bool DrawDebug = false,
		ECollisionChannel TraceChannel = ECC_Pawn);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTrace;

	/** Look for one of the caster's abilities */
	UFUNCTION(BlueprintCallable, meta = (DeterminesoutputType = "AbilityClass"), BlueprintPure)
	UAbilityBase* LookForAbility(const TSubclassOf<UAbilityBase> AbilityClass);

	/** Get the caster's passive ability */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAbilityBase* GetPassive();

#pragma region Status
protected:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadwrite, SimpleDisplay, Category = "Status", Meta = (DisplayName = "Status", ExposeFunctionCategories = "Status", AllowPrivateAccess = "true"))
	TArray<UStatusBase*> StatusToApply;
	
	/**
	 * Attempts to apply the specified status to the target.
	 * @param Target The target to apply the status to
	 * @param StatusIndex The index of the status in the StatusToApply array
	 * @param Refreshed If it wasn't a new application, but instead an already existing status was refreshed
	 * @param OverrideDuration Override the default status duration
	 * @param Duration New duration if override duration is enabled
	 * @return The newly applied status if one was applied, or the refreshed one if it already existed on the target
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UPARAM(meta = (DisplayName = Status)) UStatusBase* ApplyStatus(AC_Character* Target, int StatusIndex, bool& Refreshed, bool OverrideDuration = false, float Duration = 0.f);


	/**
	 * Searches the target's status array for a status of the specified type
	 * @param Target The target to look in
	 * @param IsDebuff Whether to search for debuff or buff
	 * @param StatusToLookFor The status class to look for
	 * @param FoundStatus The found status (null if not found)
	 * @param Stacks The stacks of the found status
	 * @param OwnOnly Only search for status applied by the caster
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable, BlueprintAuthorityOnly)
	void LookForStatus(AC_Character* Target, const bool IsDebuff, TSubclassOf<UStatusBase> StatusToLookFor, 
		UStatusBase*& FoundStatus, int& Stacks, const bool OwnOnly = true);

#pragma endregion

#pragma region Resource
protected:
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = RemoveResource))
	void BPRemoveResource(const float Amount);
	virtual void BPRemoveResource_Implementation(const float Amount) {}

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = GenerateResource))
	void BPGenerateResource(const float Resource);
	virtual void BPGenerateResource_Implementation(const float Resource) { Caster->AddResource(Resource); }

public:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastGenerateResource(const float Resource);
	void MulticastGenerateResource_Implementation(const float Resource) { BPGenerateResource(Resource); }

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastRemoveResource();
	void MulticastRemoveResource_Implementation() { BPRemoveResource(0); }

#pragma endregion

public:
	/**
	 * Get all targets found within a radius based on specified targeting criteria
	 * @param Center The center of the sphere trace
	 * @param Radius The radius of the sphere trace
	 * @param TargetsAllowed Targeting filter
	 * @param DrawDebug Draw debug visualization
	 * @return The found targets
	 */
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "DrawDebug"))
	TArray<AC_Character*> GetTargetsInRadius(const FVector Center, const float Radius, 
		UPARAM(meta = (Bitmask, BitmaskEnum = "ETargetingMask")) int32 TargetsAllowed = 2, const bool DrawDebug = false) const;


#pragma region Combat
protected:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnDealtDamage OnDealtDamage;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnHealedUnit OnHealedUnit;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintAuthorityOnly)
	void DealDamage(AC_Character* Target, FCharacterDamageEvent Event, float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool&
			IsKillingBlow, float& UnmitigatedDamage, float& DamageWithNoModifiers, float& DamageWithoutIncreases);
	virtual void DealDamage_Implementation(AC_Character* Target, FCharacterDamageEvent Event, float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool&
		IsKillingBlow, float& UnmitigatedDamage, float& DamageWithNoModifiers, float& DamageWithoutIncreases);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void HealUnit(AC_Character* Target, FCharacterDamageEvent Event, float& Healing, float& Overhealing, bool& IsCrit);

#pragma endregion

public:
	
	UFUNCTION(BlueprintNativeEvent)
	void Initialize(AActor* InCaster, int AbilitySlot);
	virtual void Initialize_Implementation(AActor* InCaster, int AbilitySlot) {};

	UFUNCTION(BlueprintCallable)
	AC_Character* GetMainTarget(TArray<AC_Character*> Targets);

#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly, category = Ability)
	AC_Character* Caster;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = Ability)
	EGameDamageType AbilitySchool;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = Ability)
	FName AbilityName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = Ability)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = Ability, meta = (Multiline = "true"))
	FText Description;
#pragma endregion
};
