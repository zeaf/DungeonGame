// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "DamageMeterWidget.generated.h"


class UDamageMeterEntry;
class AC_Character;
class UScrollBox;
/**
 * 
 */
UCLASS()
class EMPYREAN_API UDamageMeterWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageMeterEntry> EntryClass;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* MeterEntries;

	UPROPERTY()
	TMap<AC_Character*, UDamageMeterEntry*> EntriesMap;

	UPROPERTY()
	TMap<AC_Character*, float> ValuesMap;


	float CurrentMax = 0;
public:

	UFUNCTION(BlueprintCallable)
	void UpdateEntry(AC_Character* Character, const float AmountDealt);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UDamageMeterEntry* CreateEntryWidget();

	UDamageMeterEntry* CreateEntryWidget_Implementation();
	
	
};
