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
			ServerStartCooldown(Caster->GetCombatAttributeValue(CombatAttributeName::CooldownRate) * Cooldown, false);
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
	if (Duration > 0)
	{
		CooldownReady = false;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this,  &UActiveAbilityBase::ResetCooldown, Duration, false);
		ClientCooldown(Duration, IsGCD);
	}
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

void UActiveAbilityBase::StartGCD(const float Time)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimer) < Time)
		ServerStartCooldown(Time, true);
}
