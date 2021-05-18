// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitFrameBase.generated.h"

class UActiveAbilityBase;
class UImage;
class AC_Character;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class EMPYREAN_API UUnitFrameBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* HPPct;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* CurrentHP;
	UPROPERTY(BlueprintReadOnly)
		AC_Character* Target;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* Portrait;

	UFUNCTION(BlueprintCallable)
		void ClearTarget();
	UFUNCTION(BlueprintCallable)
		void TargetUpdate(AC_Character* NewTarget);

	UFUNCTION()
		void UpdateHealthEvent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateWidgetHealthValues(const FText& HPPercent, const FText& HP, float HPBarPercent);
	void UpdateWidgetHealthValues_Implementation(const FText& HPPercent, const FText& HP, float HPBarPercent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCastStart(UActiveAbilityBase* Ability, float CastTime);
	virtual void OnCastStart_Implementation(UActiveAbilityBase* Ability, float CastTime) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCastEnd();
	virtual void OnCastEnd_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartLateCastBar(UActiveAbilityBase* Ability, FDateTime CastStart, float CastTime);
	virtual void StartLateCastBar_Implementation(UActiveAbilityBase* Ability, FDateTime CastStart, float CastTime) {}

	FText GetCurrentHPText();
	float GetHPPct();
};
