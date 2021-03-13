// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAttribute.h"
#include "StatusInterface.h"
#include "GameFramework/Character.h"
#include "C_Character.generated.h"

class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathDelegate, AC_Character*, Actor);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HELENAPLAYGROUND_API AC_Character : public ACharacter, public IStatusInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_Character();

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UHealthComponent* Health;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Health")
		bool Dead;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Affiliation")
	int Team = 0;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Threat")
	float ThreatMultiplier = 1;

	UPROPERTY(BlueprintAssignable, Category = "Health")
		FDeathDelegate OnCharacterDeath;

#pragma region CombatAttributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute CriticalHitChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute CriticalHitDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute CooldownRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute Haste;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<CombatAttributeName, FCombatAttribute> CombatAttributes = {
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CriticalHitChance, CriticalHitChance),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CriticalHitDamage, CriticalHitDamage),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::Haste, Haste),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CooldownRate, CooldownRate),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::MovementSpeed, MovementSpeed) };

#pragma endregion
	
#pragma region DamageResistances
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute PhysicalResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute FireResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute FrostResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute LightningResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute VoidResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute EssenceResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute AstralResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute HolyResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<DamageType, FCombatAttribute> DamageResistance = {
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Physical, PhysicalResistance),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Fire, FireResistance),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Frost, FrostResistance),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Astral, AstralResistance),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Void, VoidResistance),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Essence, EssenceResistance),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Lightning, LightningResistance),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Holy, HolyResistance) };

#pragma endregion

#pragma region DamageFactors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute PhysicalDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute FireDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute FrostDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute LightningDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute VoidDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute EssenceDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute AstralDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute HolyDamageFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Attributes")
		FCombatAttribute AllDamageFactor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<DamageType, FCombatAttribute> DamageFactors = {
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::All, AllDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Physical, PhysicalDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Fire, FireDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Frost, FrostDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Astral, AstralDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Void, VoidDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Essence, EssenceDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Lightning, LightningDamageFactor),
		TPairInitializer<const DamageType&, const FCombatAttribute&>(DamageType::Holy, HolyDamageFactor) };

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool CheckHostility(AActor* ActorToCheck);

	void AddStatus_Implementation(UStatusBase* Status) override;

	void IRemoveStatus_Implementation(UStatusBase* Status) override;

};
