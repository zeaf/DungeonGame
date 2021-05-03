// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveAbilityBase.h"

#include "AbilityCastingComponent.h"
#include "C_Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UActiveAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	CanCastWhileMoving = CastTime == 0 || CanCastWhileMoving;
	AbilityCastingComponent = Caster->AbilityCastingComponent;
}

void UActiveAbilityBase::BPRemoveResource_Implementation()
{
	if (Cost > 0)
		Caster->RemoveResource(Cost);
}

void UActiveAbilityBase::BPServerAbilityEndCast_Implementation(AbilityCastResult CastResult)
{
	switch (CastResult)
	{
		case AbilityCastResult::Successful:
			ServerStartCooldown(GetCDAfterCdr(Cooldown), false);
			OnCastSuccess.Broadcast();
			break;
		case AbilityCastResult::Failed:
			OnCastFailed.Broadcast();
			break;
		case AbilityCastResult::Interrupted:
			OnCastInterrupted.Broadcast();
			break;
		case AbilityCastResult::Stopped:
			OnCastStopped.Broadcast();
			break;
		default: ;
	}
}

void UActiveAbilityBase::ServerStartCooldown_Implementation(const float Duration, const bool IsGCD)
{
	BPServerStartCooldown(Duration, IsGCD);
}

void UActiveAbilityBase::BPServerStartCooldown_Implementation(const float Duration, const bool IsGCD)
{
	if (Duration > 0)
	{
		//const float CD = !IsGCD && AffectedByCDR ? Duration * 
		
		CooldownReady = false;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &UActiveAbilityBase::ResetCooldown, Duration, false);
		ClientCooldown(Duration, IsGCD);
	}
}

void UActiveAbilityBase::ServerSuccessfulCastSequence_Implementation()
{
	if(!IsChanneled)
	{
		MulticastAbilityCast();
		MulticastRemoveResource();
	}

	ServerOnFinishedCast();
	ServerAbilityEndCast(AbilityCastResult::Successful);

	if (AbilityCastingComponent->CurrentlyCastingAbility)
		if (!AbilityCastingComponent->CurrentlyCastingAbility->CanCastWhileCasting)
			AbilityCastingComponent->IsCasting = false;

	BPServerSuccessfulCastSequence();
}

void UActiveAbilityBase::BPServerSuccessfulCastSequence_Implementation()
{
}

void UActiveAbilityBase::ResetCooldown()
{
	CooldownReady = true;
	ElapsedCD = 0;
}

bool UActiveAbilityBase::CastConditions_Implementation()
{
	return CheckResourceAvailability() && CooldownReady && CheckCastableWhileMoving();
}

bool UActiveAbilityBase::CheckResourceAvailability_Implementation()
{
	return true;
}

bool UActiveAbilityBase::CheckCastableWhileMoving()
{
	return CanCastWhileMoving || Caster->GetCharacterMovement()->GetLastUpdateVelocity().Equals(FVector(0, 0, 0), 1);
}

float UActiveAbilityBase::GetCDAfterCdr(const float CD)
{
	return AffectedByCDR ? Caster->GetCombatAttributeValue(CombatAttributeName::CooldownRate) * CD : CD;
}

void UActiveAbilityBase::StartGCD_Implementation(const float Time)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimer) < Time)
		ServerStartCooldown(Time, true);
}
