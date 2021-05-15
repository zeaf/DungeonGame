// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCastingComponent.h"

#include "ActiveAbilityBase.h"
#include "C_Character.h"
#include "DungeonCharacterPlayerController.h"
#include "Net/UnrealNetwork.h"

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

	if (GetOwner()->HasAuthority())
		InitializeAbilities();
	
}


// Called every frame
void UAbilityCastingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilityCastingComponent::InitializeAbilities()
{
	for (auto Ability : ActiveAbilities)
	{
		if (Ability)
		{
			UActiveAbilityBase* NewAbility = NewObject<UActiveAbilityBase>(Character, Ability);
			Character->AddOwnedComponent(NewAbility);
			NewAbility->Activate();
			NewAbility->RegisterComponent();
			NewAbility->SetComponentTickEnabled(false);
			Abilities.Add(NewAbility);
		}
	}

	if (PassiveAbilityReference)
		PassiveAbility = NewObject<UAbilityBase>(Character, PassiveAbilityReference);

	uint8 const Length = Abilities.Num();
	for (uint8 i = 0; i < Length; ++i)
	{
		if (Abilities[i])
		{
			Abilities[i]->Initialize(Character, i);
			Abilities[i]->AddTickPrerequisiteActor(Character);
		}
	}

	if (PassiveAbility) PassiveAbility->Initialize(Character, -1);
}

void UAbilityCastingComponent::ServerCastTime_Implementation(float CastTime)
{
	if (CurrentlyCastingAbility->IsChanneled)
	{
		CurrentlyCastingAbility->MulticastRemoveResource();
		CurrentlyCastingAbility->MulticastAbilityCast();	
	}

	//CurrentlyCastingAbility->ServerOnBeginCast();

	GetWorld()->GetTimerManager().SetTimer(CastingTimer, CurrentlyCastingAbility, &UActiveAbilityBase::ServerSuccessfulCastSequence,  CastTime, false);
}

void UAbilityCastingComponent::SuccessfulCastSequence()
{
	if (!CurrentlyCastingAbility->IsChanneled)
	{
		CurrentlyCastingAbility->MulticastAbilityCast();
		CurrentlyCastingAbility->MulticastRemoveResource();
	}

	CurrentlyCastingAbility->ServerOnFinishedCast();
	CurrentlyCastingAbility->ServerAbilityEndCast(AbilityCastResult::Successful);
	if (!CurrentlyCastingAbility->CanCastWhileCasting)
		IsCasting = false;
	
}

void UAbilityCastingComponent::ServerAttemptToCast_Implementation(UActiveAbilityBase* Ability)
{
	if (!Ability) return;
	
	if (!IsCasting || Ability->CanCastWhileCasting)
	{
		if (Ability->CastConditions())
		{
			AbilityCastResult Result;
			Ability->ServerOnBeginCast(Result);
			TriggerGCD(Character->GetCombatAttributeValue(CombatAttributeName::CooldownRate) * Ability->GCD);

			const float CastTime = GetCastTimeAfterHaste(Ability->CastTime);

			if (Result == AbilityCastResult::Successful)
			{
				if (CastTime > 0)
				{
					ClientCastbar(Ability, CastTime);
					if (!Ability->CanCastWhileCasting)
					{
						IsCasting = true;
						CanCastWhileMoving = true;
						CurrentlyCastingAbility = Ability;
						ServerCastTime(CastTime);
					}
				}
				else
					Ability->ServerSuccessfulCastSequence();
			}
			else
				Ability->ServerAbilityEndCast(Result);
		}
	}
	else // Queue ability
	{
		if (GetWorld()->GetTimerManager().GetTimerRemaining(CastingTimer) <= SpellQueueWindow)
		{
			if (CurrentlyCastingAbility)
			{
				CurrentlyCastingAbility->OnCastSuccess.Remove(this, FName("CastQueuedAbility"));
				QueuedAbility = Ability;
				CurrentlyCastingAbility->OnCastSuccess.AddDynamic(this, &UAbilityCastingComponent::CastQueuedAbility);
			}
		}
	}
}

void UAbilityCastingComponent::ServerInterruptCast_Implementation()
{
	if (IsCasting)
	{
		CastingTimer.Invalidate();
		IsCasting = false;
		CanCastWhileMoving = false;
		Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
		//if (Character->GetPlayerController())
		//	Character->GetPlayerController()->
		if (IsValid(CurrentlyCastingAbility))
			CurrentlyCastingAbility->MulticastOnInterrupted();
	}
}

void UAbilityCastingComponent::ClientCastbar_Implementation(const UActiveAbilityBase* Ability, const float CastTime)
{
	if (Character->GetPlayerController())
		Character->GetPlayerController()->Castbar(Ability, CastTime);
}

void UAbilityCastingComponent::TriggerGCD(const float Time)
{
	for (UActiveAbilityBase* Ability : Abilities)
		Ability->StartGCD(GetCastTimeAfterHaste(Time));
}

float UAbilityCastingComponent::GetCastTimeAfterHaste(const float CastTime)
{
	return FMath::Max<float>((1 - Character->GetCombatAttributeValue(CombatAttributeName::Haste) / 100.) * CastTime, CastTime/2);
}

void UAbilityCastingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAbilityCastingComponent, Abilities);
}

