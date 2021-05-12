// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CharacterDamageEvent.h"
#include "CombatAttribute.h"
#include "StatusInterface.h"
#include "GameFramework/Character.h"
#include "C_Character.generated.h"

class ADungeonCharacterPlayerController;
class UAbilityCastingComponent;
class UStatusComponent;
class UHealthComponent;
struct FCharacterDamageEvent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathDelegate, AC_Character*, Actor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResourceUpdatedDelegate, float, CurrentResource, float, MaxResource);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMPYREAN_API AC_Character : public ACharacter, public IStatusInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Character();

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UHealthComponent* Health;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStatusComponent* StatusComponent;
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAbilityCastingComponent* AbilityCastingComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Health")
	bool Dead = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Affiliation")
	int Team = 0;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Threat")
	float ThreatMultiplier = 1;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Health")
	FDeathDelegate OnCharacterDeath;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health")
	void OnHealingReceived(FCharacterDamageEvent Event, float& Healing, bool& IsCrit);
	virtual void OnHealingReceived_Implementation(FCharacterDamageEvent Event, float& Healing, bool& IsCrit);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health")
	void OnDamageReceived(FCharacterDamageEvent Event, float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool& IsKillingBlow);
	virtual void OnDamageReceived_Implementation(FCharacterDamageEvent Event, float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool& IsKillingBlow);

#pragma region CombatAttributes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<CombatAttributeName, FCombatAttribute> CombatAttributes = {
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CriticalHitChance, FCombatAttribute(15.f)),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CriticalHitDamage, FCombatAttribute(2.f)),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::Haste, FCombatAttribute(0.f)),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CooldownRate, FCombatAttribute()),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::MovementSpeed, FCombatAttribute()),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::HealingDone, FCombatAttribute()),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::HealingTaken, FCombatAttribute()) };

#pragma endregion
	
#pragma region DamageResistances
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<EGameDamageType, FCombatAttribute> DamageResistance = {
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::All, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Physical, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Fire, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Frost, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Astral, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Void, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Essence, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Lightning, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Holy, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Nature, FCombatAttribute()) };

#pragma endregion

#pragma region DamageFactors
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<EGameDamageType, FCombatAttribute> DamageFactors = {
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::All, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Physical, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Fire, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Frost, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Astral, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Void, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Essence, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Lightning, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Holy, FCombatAttribute()),
		TPairInitializer<const EGameDamageType&, const FCombatAttribute&>(EGameDamageType::Nature, FCombatAttribute()) };

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintGetter= GetPlayerController)
	ADungeonCharacterPlayerController* DungeonPlayerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	ADungeonCharacterPlayerController* GetPlayerController() { return DungeonPlayerController; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPARAM(DisplayName="IsEnemy") bool CheckHostility(AActor* ActorToCheck);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCombatAttributeValue(CombatAttributeName Attribute);

	virtual UStatusBase* AddStatus_Implementation(UStatusBase* Status, AC_Character* Caster, UAbilityBase* Ability, bool& Refreshed, bool OverrideDuration = false, float Duration = 0.f) override;

	void IRemoveStatus_Implementation(UStatusBase* Status) override;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void OnDeath();
	virtual void OnDeath_Implementation();

#pragma region Resource
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FResourceUpdatedDelegate OnResourceUpdated;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastOnResourceUpdated(const float Current, const float Max);
	void MulticastOnResourceUpdated_Implementation(const float Current, const float Max) { OnResourceUpdated.Broadcast(Current, Max); }
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Resource")
	void RemoveResource(const float Resource);
	virtual void RemoveResource_Implementation(const float Resource) {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Resource")
	void AddResource(const float Resource);
	virtual void AddResource_Implementation(const float Resource) {};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "Resource")
	void GetResource(float& Resource, float& MaxResource, float& ResourcePct);
	virtual void GetResource_Implementation(float& Resource, float& MaxResource, float& ResourcePct) { Resource = 0; ResourcePct = 0; MaxResource = 0; };

#pragma endregion 
};
