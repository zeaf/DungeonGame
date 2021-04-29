// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCastingComponent.h"

#include "ActiveAbilityBase.h"
#include "C_Character.h"

// Sets default values for this component's properties
UAbilityCastingComponent::UAbilityCastingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	Character = Cast<AC_Character>(GetOwner());
}


// Called when the game starts
void UAbilityCastingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAbilityCastingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilityCastingComponent::ServerCastTime_Implementation()
{
	if (CurrentlyCastingAbility->IsChanneled)
	{
		CurrentlyCastingAbility->MulticastRemoveResource();
		CurrentlyCastingAbility->MulticastAbilityCast();	
	}

	CurrentlyCastingAbility->ServerOnBeginCast();

	GetWorld()->GetTimerManager().SetTimer(CastingTimer, this, &UAbilityCastingComponent::SuccessfulCastSequence, CurrentlyCastingAbility->CastTime, false);
}

void UAbilityCastingComponent::SuccessfulCastSequence()
{
	if (CurrentlyCastingAbility->IsChanneled)
	{
		CurrentlyCastingAbility->MulticastAbilityCast();
		CurrentlyCastingAbility->MulticastRemoveResource();
	}

	CurrentlyCastingAbility->ServerOnFinishedCast();
	
}

void UAbilityCastingComponent::ServerAttemptToCast_Implementation(UActiveAbilityBase* Ability)
{
	if (!IsCasting || CanCastWhileCasting)
	{
		if (Ability->CastConditions())
		{
			TriggerGCD(Character->GetCombatAttributeValue(CombatAttributeName::CooldownRate) * Ability->GCD);

			if (Ability->CastTime > 0)
			{
				ClientCastbar(Ability);
				if (!Ability->CanCastWhileCasting)
				{
					IsCasting = true;
					CanCastWhileMoving = true;
					CurrentlyCastingAbility = Ability;
					ServerCastTime();
				}
			}
			else
				SuccessfulCastSequence();
		}
	}
}

void UAbilityCastingComponent::ClientCastbar_Implementation(const UActiveAbilityBase* Ability)
{
	
}

void UAbilityCastingComponent::TriggerGCD(const float Time)
{
	for (UActiveAbilityBase* Ability : ActiveAbilities)
		Ability->StartGCD(Time);
}

