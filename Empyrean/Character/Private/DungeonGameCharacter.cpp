// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGameCharacter.h"


#include "Chaos/AABB.h"
#include "Chaos/AABB.h"

void UDungeonGameCharacter::AddAbilityToCharacter(AActor* Character, UActorComponent* Ability, TSubclassOf<UAbilityBase> AbilityClass, UClass* A)
{
	Character->AddOwnedComponent(Ability);
	Ability->Activate();
	Ability->RegisterComponent();
	Ability->SetComponentTickEnabled(true);
}

UAbilityBase* UDungeonGameCharacter::CreateAbility(AActor* Character, class TSubclassOf<class UAbilityBase> AbilityClass)
{
	if (AbilityClass)
	{
		UAbilityBase* NewAbility = NewObject<UAbilityBase>(Character, AbilityClass);
		Character->AddOwnedComponent(NewAbility);
		NewAbility->Activate();
		NewAbility->RegisterComponent();
		NewAbility->SetComponentTickEnabled(false);
		return NewAbility;
	}
	return nullptr;
}

void UDungeonGameCharacter::InitializeAbilities(AActor* Character, UPARAM(ref) TArray<UAbilityBase*>& AbilitiesArray)
{
	uint8 const Length = AbilitiesArray.Num();
	for (uint8 i = 0; i < Length; ++i)
	{
		if (AbilitiesArray[i])
		{
			AbilitiesArray[i]->Initialize(Character, i);
			AbilitiesArray[i]->AddTickPrerequisiteActor(Character);
		}
	}
}

void UDungeonGameCharacter::DestroyController(AController* Controller)
{
	Controller->UnPossess();
	Controller->SetActorTickEnabled(false);
	Controller->Destroy();
}
