// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitNameplate.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class EMPYREAN_API UUnitNameplate : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UCastbar* Castbar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* Canvas;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UDebuffList* Auras;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* Healthbar;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* HPPercentage;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	class AC_Character* Character;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateHealthEvent(float Current, float Max,float Percentage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddFloatingCombatText(UUserWidget* FCTWidget, float Value, bool IsCrit, bool IsHealing, bool IsAbsorb);
	void AddFloatingCombatText_Implementation(UUserWidget* FCTWidget, float Value, bool IsCrit, bool IsHealing, bool IsAbsorb);
	
protected:
	void SetHPPercentageText(float Percentage);
};
