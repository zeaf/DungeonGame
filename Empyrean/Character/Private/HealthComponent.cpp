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
	CurrentHealth = MaxHealth.GetFinalValue();
	MaxHealableHealth = FCombatAttribute(MaxHealth.GetFinalValue());
	MaxHealth.OnValueChange.AddDynamic(this, &UHealthComponent::OnMaxHealthUpdate);
}

void UHealthComponent::OnMaxHealthUpdate(float NewMaxHealth)
{
	if (CurrentHealth > MaxHealth.CurrentValue)
		CurrentHealth = MaxHealth.CurrentValue;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetHealth(float& Current, float& Max, float& MaxHealable, float& HealthPercentage)
{
	Current = CurrentHealth;
	Max = MaxHealth.CurrentValue;
	MaxHealable = MaxHealableHealth.CurrentValue;
	HealthPercentage = CurrentHealth / Max;
}

float UHealthComponent::GetDamageFactorForType(AC_Character* DamageDealer, EGameDamageType Type)
{
	return DamageDealer ?
		DamageDealer->DamageFactors[EGameDamageType::All].GetFinalValue() * DamageDealer->DamageFactors[Type].GetFinalValue()
		: 1.f;
}

float UHealthComponent::GetCritMultiplier(AC_Character* DamageDealer, 
	const float IncreasedCriticalChance, const float IncreasedCriticalDamage, bool& IsCrit)
{
	IsCrit = CheckCriticalHit(DamageDealer, IncreasedCriticalChance);
	const float Amount = IsCrit ? DamageDealer->CombatAttributes[ECombatAttributeName::CriticalHitDamage].GetFinalValue() + IncreasedCriticalDamage
		: 1;
	return Amount;
}

bool UHealthComponent::CheckCriticalHit(AC_Character* DamageDealer, const float IncreasedCriticalChance)
{
	return FMath::FRandRange(0, 100) <= (DamageDealer->CombatAttributes[ECombatAttributeName::CriticalHitChance].GetFinalValue() + IncreasedCriticalChance);
}

float UHealthComponent::CalculateDamageReduction(EGameDamageType Type)
{
	return Pawn->DamageResistance[Type].GetFinalValue() * Pawn->DamageResistance[EGameDamageType::All].GetFinalValue();
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
		RemovedAbsorb->Status->Expired(true);

	AbsorbedDamage = IncomingDamage - Damage;
	NotAbsorbedDamage = Damage;
}

FDamageOutcome UHealthComponent::OnHit(FCharacterDamageEvent DamageEvent)
{
	FDamageOutcome Outcome = FDamageOutcome();

	if (Pawn->Dead)
		return Outcome;

	float TotalMultiplier = 1.f;

	const float Variance = DamageEvent.ApplyVariance ? FMath::FRandRange(VARIANCE_LOW, VARIANCE_HIGH) : 1.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"),Variance));
	// Amount increase based on damage type, and variance
	// Critical hit calculation
	const float CritMultiplier = GetCritMultiplier(DamageEvent.Instigator, DamageEvent.AdditionalCriticalChance,
		DamageEvent.AdditionalCriticalDamage, Outcome.bIsCrititcal);

	const float DamageIncreaseMultiplier = GetDamageFactorForType(DamageEvent.Instigator, DamageEvent.Type);

	const float DamageReductionMultiplier = CalculateDamageReduction(DamageEvent.Type);

	const float DamageAfterVariance = DamageEvent.Amount * Variance;
		
	Outcome.DamageWithNoModifiers = DamageAfterVariance * CritMultiplier;

	Outcome.DamageWithoutIncreases = Outcome.DamageWithNoModifiers * DamageReductionMultiplier;

	Outcome.UnmitigatedDamage = Outcome.DamageWithNoModifiers * DamageIncreaseMultiplier;

	TotalMultiplier = Variance * CritMultiplier * DamageIncreaseMultiplier * DamageReductionMultiplier;

	Outcome.DamageTaken = FMath::CeilToFloat(DamageEvent.Amount * TotalMultiplier);

	CheckForAbsorbs(Outcome.DamageTaken, Outcome.DamageAbsorbed, Outcome.DamageTaken);
	
	// Damage the target based on the calculated damage value
	if (Outcome.DamageTaken > 0)
	{
		Outcome.bIsKillingBlow = DamageCharacter(Outcome.DamageTaken);

		OnDamageReceived.Broadcast(DamageEvent.Instigator, Outcome);
	}

	return Outcome;
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::MulticastReduceHealth_Implementation(float IncomingDamage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - IncomingDamage, MinHealth, MaxHealth.GetFinalValue());
	UpdateHealth.Broadcast(CurrentHealth, MaxHealth.CurrentValue, CurrentHealth / MaxHealth.CurrentValue);
}

bool UHealthComponent::DamageCharacter_Implementation(float IncomingDamage)
{
	MulticastReduceHealth(IncomingDamage);
	
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
	CurrentHealth = FMath::Clamp(CurrentHealth + IncomingHealing, 0.f, MaxHealableHealth.GetFinalValue());
	UpdateHealth.Broadcast(CurrentHealth, MaxHealth.CurrentValue, CurrentHealth / MaxHealth.CurrentValue);
}


void UHealthComponent::MulticastSetHealth_Implementation(float NewHealth)
{
	MaxHealth.SetValue(NewHealth);
	CurrentHealth = NewHealth;
	MaxHealableHealth.SetValue(NewHealth);
	UpdateHealth.Broadcast(CurrentHealth, MaxHealth.CurrentValue, CurrentHealth / MaxHealth.CurrentValue);
}

FHealingOutcome UHealthComponent::OnHealReceived(FCharacterDamageEvent HealingEvent)
{
	FHealingOutcome Outcome = FHealingOutcome();

	if (Pawn->Dead)
		return Outcome;

	const float Variance = HealingEvent.ApplyVariance ? FMath::FRandRange(VARIANCE_LOW, VARIANCE_HIGH) : 1.0f;

	float TotalMultiplier = 1.f;
	
	TotalMultiplier *= GetHealingMultiplier(HealingEvent.Instigator) * Variance;

	TotalMultiplier *= GetCritMultiplier(HealingEvent.Instigator, HealingEvent.AdditionalCriticalChance,
	HealingEvent.AdditionalCriticalDamage, Outcome.bIsCrititcal);

	Outcome.HealingTaken = FMath::CeilToFloat(HealingEvent.Amount * TotalMultiplier);
	
	if (Outcome.HealingTaken > 0)
	{
		Outcome.Overhealing = Outcome.HealingTaken - Pawn->Health->MaxHealableHealth.CurrentValue + Pawn->Health->CurrentHealth;
		MulticastRestoreHealth(Outcome.HealingTaken);
		OnHealingReceived.Broadcast(HealingEvent.Instigator, Outcome);
	}

	return Outcome;
}

float UHealthComponent::GetHealingMultiplier(AC_Character* Healer)
{
	return Healer->CombatAttributes[ECombatAttributeName::HealingDone].GetFinalValue() * Pawn->CombatAttributes[ECombatAttributeName::HealingTaken].GetFinalValue();
}
