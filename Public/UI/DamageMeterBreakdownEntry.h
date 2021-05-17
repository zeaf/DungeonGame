// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageMeterBreakdownEntry.generated.h"

class UAbilityBase;
class UProgressBar;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class EMPYREAN_API UDamageMeterBreakdownEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Value;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Percentage;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Bar;

	UPROPERTY(BlueprintReadOnly)
	float CurrentValue;

	float BarFillPercentage = 0;

	UFUNCTION()
	void UpdateBreakdownEntry(float CurrentMax, float TotalDamage);

	UFUNCTION()
	void InitializeEntry(UAbilityBase* Ability, float Amount, float CurrentMax, float TotalDamage);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
