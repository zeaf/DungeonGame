// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageMeterEntryBreakdown.generated.h"

class UWrapBox;
class UDamageMeterBreakdownEntry;
class UAbilityBase;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class UDamageMeterEntryBreakdown : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UWrapBox* EntryList;

	void ShiftChild(UWrapBox* WrapBox, int32 Index, UWidget* Child);

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<int, UDamageMeterBreakdownEntry*> Entries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageMeterBreakdownEntry> EntryClass;

	UPROPERTY(BlueprintReadOnly)
	TMap<int, float> EntryValues;
	
	UFUNCTION(BlueprintCallable)
		void ShowBreakdown();

	float TotalDamage = 0;

	float CurrentMax = 0;

	UFUNCTION()
	void UpdateEntry(UAbilityBase* Ability, float Amount, float TotalCharacterDamage);

};
