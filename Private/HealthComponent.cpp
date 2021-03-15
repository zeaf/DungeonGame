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

	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Pawn = Cast<AC_Character>(GetOuter());
	CurrentHealth = MaxHealth;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetDamageFactorForType(AC_Character* DamageDealer, GameDamageType Type)
{
	return DamageDealer ?
		DamageDealer->DamageFactors[GameDamageType::All].GetFinalValue() * DamageDealer->DamageFactors[Type].GetFinalValue()
		: 1.f;
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

float UHealthComponent::CalculateDamageReduction(GameDamageType Type, const float IncomingDamage)
{
	return Pawn->DamageResistance[Type].GetFinalValue() * Pawn->DamageResistance[GameDamageType::All].GetFinalValue() * IncomingDamage;
}

void UHealthComponent::CheckForAbsorbs(const float IncomingDamage, float& AbsorbedDamage, float& NotAbsorbedDamage)
{
	float Damage = IncomingDamage;

	TArray<UEffectAbsorbDamage*> AbsorbsRemoved;
	
	for (UEffectAbsorbDamage* AbsorbBuff : Absorbs)
	{
		if (AbsorbBuff->AbsorbAmount > Damage)
		{
			AbsorbBuff->AbsorbAmount -= Damage;
			Damage = 0;
			break;
		}
		Damage -= AbsorbBuff->AbsorbAmount;
		AbsorbsRemoved.Add(AbsorbBuff);
	}

	for (UEffectAbsorbDamage* RemovedAbsorb : AbsorbsRemoved)
		RemovedAbsorb->Status->Expired();

	AbsorbedDamage = IncomingDamage - Damage;
	NotAbsorbedDamage = Damage;
}

void UHealthComponent::OnHit(FCharacterDamageEvent DamageEvent, float& FinalDamageTaken,
	float& DamageAbsorbed, bool& IsCrit, bool& IsKillingBlow)
{
	if (Pawn->Dead)
		return;

	FinalDamageTaken = 0;

	const float Variance = DamageEvent.ApplyVariance ? FMath::FRandRange(VARIANCE_LOW, VARIANCE_HIGH) : 1.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"),Variance));
	// Amount increase based on damage type, and variance
	FinalDamageTaken = GetDamageFactorForType(DamageEvent.Instigator, DamageEvent.Type) * DamageEvent.Amount * Variance;

	// Critical hit calculation
	FinalDamageTaken = DamageAfterCritCalculation(DamageEvent.Instigator, FinalDamageTaken,
		DamageEvent.AdditionalCriticalChance, DamageEvent.AdditionalCriticalDamage, IsCrit);

	FinalDamageTaken = CalculateDamageReduction(DamageEvent.Type, FinalDamageTaken);
	
	CheckForAbsorbs(FinalDamageTaken, DamageAbsorbed, FinalDamageTaken);

	// Damage the target based on the calculated damage value
	if (FinalDamageTaken > 0)
		IsKillingBlow = DamageCharacter(FinalDamageTaken);
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::MulticastReduceHealth_Implementation(float IncomingDamage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - IncomingDamage, MinHealth, MaxHealth);
	UpdateHealth.Broadcast();
}

bool UHealthComponent::DamageCharacter_Implementation(float IncomingDamage)
{
	MulticastReduceHealth(IncomingDamage);

	OnDamageReceived.Broadcast(Pawn, IncomingDamage);
	
	if (CurrentHealth == 0)
	{
		Pawn->OnDeath();
		
		AAIController* AI = Cast<AAIController>(Pawn->GetController());
		
		if (AI) AI->Destroy();
	}
	return CurrentHealth == 0;
}

void UHealthComponent::MulticastRestoreHealth_Implementation(float IncomingHealing)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + IncomingHealing, 0.f, MaxHealableHealth);
	UpdateHealth.Broadcast();
}

void UHealthComponent::OnHealReceived(FCharacterDamageEvent HealingEvent, float& FinalHealingTaken, bool& IsCrit)
{
	if (Pawn->Dead)
		return;

	FinalHealingTaken = 0;

	const float Variance = HealingEvent.ApplyVariance ? FMath::FRandRange(VARIANCE_LOW, VARIANCE_HIGH) : 1.0f;
	
}

float UHealthComponent::GetHealingMultiplier(AC_Character* Healer)
{
	return Healer->HealingDone.GetFinalValue() * Pawn->HealingTaken.GetFinalValue();
}
