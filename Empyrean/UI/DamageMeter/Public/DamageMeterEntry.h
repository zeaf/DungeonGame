// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageMeterEntry.generated.h"

class UAbilityBase;
class UDamageMeterEntryBreakdown;
class AC_Character;
class UTextBlock;
class UProgressBar;

UCLASS()
class EMPYREAN_API UDamageMeterEntry : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* Bar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* NameBlock;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Amount;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PerSecondValue;

	UPROPERTY(BlueprintReadonly, meta = (BindWidget))
	UDamageMeterEntryBreakdown* Breakdown;
	
	TWeakObjectPtr<AC_Character> Character;

	UPROPERTY()
		float TotalAmountDealt;

	float BarFillPercent;

	FDateTime StartTime;
	FTimerHandle PerSecondUpdateTimer;

	UFUNCTION()
	void UpdatePerSecondValue();
	
	UFUNCTION(BlueprintCallable)
	void InitializeDamageMeterEntry(AC_Character* Instigator, const float AmountDealt, const float CurrentMax, UAbilityBase* Ability);

	UFUNCTION(BlueprintCallable)
	void UpdateValue(const float AmountDealt, const float CurrentMax, UAbilityBase* Ability);

	UFUNCTION(BlueprintCallable)
	void UpdateBarPercent(const float CurrentMax);

	void UpdateDynamicInformation(const float InDeltaTime);
};
