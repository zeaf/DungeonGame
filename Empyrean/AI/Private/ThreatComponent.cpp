// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreatComponent.h"

// Sets default values for this component's properties
UThreatComponent::UThreatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UThreatComponent::AddThreat(AC_Character* DamageDealer, const FDamageOutcome Outcome)
{
	if (ThreatMap.Find(DamageDealer))
		ThreatMap[DamageDealer] += Outcome.DamageTaken * DamageDealer->ThreatMultiplier;
	else
	{
		ThreatMap.Emplace(DamageDealer, Outcome.DamageTaken * DamageDealer->ThreatMultiplier);
		DamageDealer->OnCharacterDeath.AddDynamic(this, &UThreatComponent::RemoveFromThreatMap);
	}
	
	ThreatMap.ValueSort([](float A, float B) {
		return A > B; });
}

void UThreatComponent::RemoveFromThreatMap(AC_Character* Character)
{
	if (Character)
		if (ThreatMap.Find(Character))
		{
			ThreatMap.Remove(Character);
			Character->OnCharacterDeath.RemoveDynamic(this, &UThreatComponent::RemoveFromThreatMap);
		}
}

// Called when the game starts
void UThreatComponent::BeginPlay()
{
	Super::BeginPlay();
	Pawn = Cast<AC_Character>(GetOwner());
	HealthComp = Cast<UHealthComponent>(Pawn->GetComponentByClass(UHealthComponent::StaticClass()));

	HealthComp->OnDamageReceived.AddDynamic(this, &UThreatComponent::AddThreat);
	
}

AC_Character* UThreatComponent::GetHighestThreatActor()
{
	if (ThreatMap.Num() > 0)
	{
		ThreatMap.ValueSort([](float A, float B){
				return A > B; });

		return ThreatMap.begin().Key();		
	}
	return nullptr;
}


// Called every frame
void UThreatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

