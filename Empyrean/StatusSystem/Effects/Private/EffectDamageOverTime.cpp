// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDamageOverTime.h"

#include "AbilityBase.h"
#include "C_Character.h"
#include "StatusBase.h"

void UEffectDamageOverTime::Initialize_Implementation(UStatusBase* ParentStatus)
{
	Super::Initialize_Implementation(ParentStatus);

	DamagePerTick = TotalDamage / NumberOfTicks;

	Event.Ability = Status->Ability;
	Event.Instigator = Status->Instigator;
	Event.AdditionalCriticalChance = AdditionalCriticalChance;
	Event.AdditionalCriticalDamage = AdditionalCriticalDamage;
	Event.Amount = DamagePerTick;
	Event.IsPeriodic = true;
	Event.Type = Type;
}

void UEffectDamageOverTime::OnTick_Implementation()
{
	if (!IsValid(Status->TargetActor)) return;
	
	Super::OnTick_Implementation();
	Event.Amount = DamagePerTick * Status->CurrentStacks;
	Status->Ability->DealDamage(Status->TargetActor, Event);
}

void UEffectDamageOverTime::OnRefreshed_Implementation()
{
	if (KeepPreviousDamage)
	{
		float UndealtDamage = DamagePerTick * (NumberOfTicks - TicksDone);
		DamagePerTick = (TotalDamage + UndealtDamage) / NumberOfTicks;
	}
	Super::OnRefreshed_Implementation();

}
