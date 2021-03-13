// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "C_Character.h"
#include "EffectAbsorbDamage.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetDamageFactorForType(TEnumAsByte<DamageType> Type)
{
	return Pawn->DamageFactors[DamageType::All].GetFinalValue() * Pawn->DamageFactors[Type].GetFinalValue();
}

float UHealthComponent::DamageAfterCritCalculation(AC_Character* DamageDealer, 
	const float IncomingDamage, const float IncreasedCriticalChance, const float IncreasedCriticalDamage)
{
	return CheckCriticalHit(DamageDealer, IncreasedCriticalChance) ?
		DamageDealer->CriticalHitDamage.GetFinalValue() + IncreasedCriticalDamage * IncomingDamage
		: IncomingDamage;
}

bool UHealthComponent::CheckCriticalHit(AC_Character* DamageDealer, const float IncreasedCriticalChance)
{
	return FMath::FRandRange(0, 100) <= (DamageDealer->CriticalHitChance.GetFinalValue() + IncreasedCriticalChance);
}

float UHealthComponent::CalculateDamageReduction(TEnumAsByte<DamageType> Type, const float IncomingDamage)
{
	return Pawn->DamageResistance[Type].GetFinalValue() * Pawn->DamageResistance[DamageType::All].GetFinalValue() * IncomingDamage;
}

void UHealthComponent::CheckForAbsorbs(const float IncomingDamage, float& AbsorbedDamage, float& NotAbsorbedDamage)
{
	float Damage = IncomingDamage;
	for (UEffectAbsorbDamage* AbsorbBuff : Absorbs)
	{
		if (AbsorbBuff->AbsorbAmount > Damage)
		{
			AbsorbBuff->AbsorbAmount -= Damage;
			Damage = 0;
			break;
		}
		Damage -= AbsorbBuff->AbsorbAmount;
		AbsorbBuff->Status->Expired();
	}

	AbsorbedDamage = IncomingDamage - Damage;
	NotAbsorbedDamage = Damage;
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}
