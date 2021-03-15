// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectHealingOverTime.h"
#include "C_Character.h"
#include "StatusBase.h"

void UEffectHealingOverTime::Initialize(UStatusBase* ParentStatus)
{
	Super::Initialize(ParentStatus);

	HealingPerTick = TotalHealing / NumberOfTicks;

	Event.Ability = Status->Ability;
	Event.Instigator = Status->Instigator;
	Event.AdditionalCriticalChance = AdditionalCriticalChance;
	Event.AdditionalCriticalDamage = AdditionalCriticalDamage;
	Event.Amount = HealingPerTick;
	Event.IsPeriodic = true;
	Event.Type = Type;
}

void UEffectHealingOverTime::OnTick_Implementation()
{
	Super::OnTick_Implementation();
	Status->TargetActor->OnHealingReceived(Event);
}