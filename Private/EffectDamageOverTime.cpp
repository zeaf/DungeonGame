// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDamageOverTime.h"
#include "C_Character.h"
#include "StatusBase.h"

void UEffectDamageOverTime::Initialize_Implementation(UStatusBase* ParentStatus)
{
	Super::Initialize_Implementation(ParentStatus);

	DamagePertTick = TotalDamage / NumberOfTicks;

	Event.Ability = Status->Ability;
	Event.Instigator = Status->Instigator;
	Event.AdditionalCriticalChance = AdditionalCriticalChance;
	Event.AdditionalCriticalDamage = AdditionalCriticalDamage;
	Event.Amount = DamagePertTick;
	Event.IsPeriodic = true;
	Event.Type = Type;
}

void UEffectDamageOverTime::OnTick_Implementation()
{
	Super::OnTick_Implementation();
	Event.Amount = DamagePertTick * Status->CurrentStacks;
	float D, A;
	bool C, K;
	Status->TargetActor->OnDamageReceived(Event, D, A, C, K);
}
