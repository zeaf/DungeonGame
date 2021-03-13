// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "AIController.h"
#include "Net/UnrealNetwork.h"
#include "C_Character.h"
#include "EffectAbsorbDamage.h"


#define VARIANCE_LOW 0.98
#define VARIANCE_HIGH 1.02

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
	const float IncomingDamage, const float IncreasedCriticalChance, const float IncreasedCriticalDamage, bool& IsCrit)
{
	IsCrit = CheckCriticalHit(DamageDealer, IncreasedCriticalChance);
	return  IsCrit ? DamageDealer->CriticalHitDamage.GetFinalValue() + IncreasedCriticalDamage * IncomingDamage
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

void UHealthComponent::OnHit(FCharacterDamageEvent* DamageEvent, float& FinalDamageTaken, bool& IsCrit, bool& IsKillingBlow)
{
	if (Pawn->Dead)
		return;

	FinalDamageTaken = 0;

	// Amount increase based on damage type, and variance
	FinalDamageTaken = GetDamageFactorForType(DamageEvent->Type)
		* DamageEvent->Amount
		* DamageEvent->ApplyVariance ? FMath::FRandRange(VARIANCE_LOW, VARIANCE_HIGH) : 1;

	// Critical hit calculation
	FinalDamageTaken = DamageAfterCritCalculation(DamageEvent->Instigator, FinalDamageTaken,
		DamageEvent->AdditionalCriticalChance, DamageEvent->AdditionalCriticalDamage, IsCrit);

	float AbsorbedDamage, NotAbsorbedDamage;
	CheckForAbsorbs(FinalDamageTaken, AbsorbedDamage, NotAbsorbedDamage);
	
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::ReduceHealth_Implementation(float IncomingDamage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - IncomingDamage, 0.f, MaxHealth);
	
}

bool UHealthComponent::ServerDamageCharacter_Implementation(float IncomingDamage)
{
	ReduceHealth(IncomingDamage);

	if (CurrentHealth == 0)
	{
		Pawn->OnDeath();
		
		AAIController* AI = Cast<AAIController>(Pawn->GetController());
		
		if (AI) AI->Destroy();
	}
	return CurrentHealth == 0;
}
