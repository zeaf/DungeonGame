// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilityBase.h"
#include "DungeonGameCharacter.generated.h"


/**
 * 
 */
UCLASS()
class HELENAPLAYGROUND_API UDungeonGameCharacter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "DungeonGame")
	static void AddAbilityToCharacter(AActor* Character, UActorComponent* Ability, TSubclassOf<UAbilityBase> AbilityClass, UClass* A);

	UFUNCTION(BlueprintCallable, Category = "DungeonGame", meta = (DeterminesoutputType = "AbilityClass"))
	static class UAbilityBase* CreateAbility(AActor* Character, TSubclassOf<UAbilityBase> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "DungeonGame")
	static void InitializeAbilities(AActor* Character, UPARAM(ref) TArray<UAbilityBase*>& AbilitiesArray);

	UFUNCTION(BlueprintCallable, Category = "DungeonGame")
	static void DestroyController(AController* Controller);
};
