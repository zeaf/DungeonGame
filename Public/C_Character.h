// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAttribute.h"
#include "StatusInterface.h"
#include "GameFramework/Character.h"
#include "C_Character.generated.h"

class UStatusComponent;
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
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStatusComponent* StatusComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Health")
		bool Dead = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Affiliation")
	int Team = 0;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Threat")
	float ThreatMultiplier = 1;

	UPROPERTY(BlueprintAssignable, Category = "Health")
		FDeathDelegate OnCharacterDeath;

#pragma region CombatAttributes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<CombatAttributeName, FCombatAttribute> CombatAttributes = {
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CriticalHitChance, FCombatAttribute(15.f)),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CriticalHitDamage, FCombatAttribute(2.f)),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::Haste, FCombatAttribute()),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::CooldownRate, FCombatAttribute()),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::MovementSpeed, FCombatAttribute()),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::HealingDone, FCombatAttribute()),
		TPairInitializer<const CombatAttributeName&, const FCombatAttribute&>(CombatAttributeName::HealingTaken, FCombatAttribute()) };

#pragma endregion
	
#pragma region DamageResistances
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<GameDamageType, FCombatAttribute> DamageResistance = {
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Physical, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Fire, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Frost, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Astral, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Void, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Essence, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Lightning, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Holy, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::All, FCombatAttribute())};

#pragma endregion

#pragma region DamageFactors
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NoClear, Category = "Attributes")
		TMap<GameDamageType, FCombatAttribute> DamageFactors = {
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::All, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Physical, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Fire, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Frost, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Astral, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Void, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Essence, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Lightning, FCombatAttribute()),
		TPairInitializer<const GameDamageType&, const FCombatAttribute&>(GameDamageType::Holy, FCombatAttribute()) };

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

	UStatusBase* AddStatus_Implementation(UStatusBase* Status) override;

	void IRemoveStatus_Implementation(UStatusBase* Status) override;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
		void OnDeath();
	virtual void OnDeath_Implementation();

	

};
