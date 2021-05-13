// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Widget_SpellIcon.generated.h"



class UStatusBase;
UCLASS()
class EMPYREAN_API UWidget_SpellIcon : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* AbilityIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* NoResourceOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* IconStyleOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* RadialSwipe;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* RadialSwipeCharge;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CooldownTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* StacksText;

	UPROPERTY()
	int CurrentStacks;


public:
	UFUNCTION(BlueprintCallable)
	void UpdateStacksDisplay(int Stacks, bool Force=false);

	UFUNCTION(BlueprintNativeEvent)
	void PlayRadialSwipe(const float StartAtTime, const float Duration);
	void PlayRadialSwipe_Implementation(const float StartAtTime, const float Duration) {}

	UFUNCTION(BlueprintCallable)
	void InitializeIcon(UStatusBase* Status, UTexture2D* Icon, float Duration);
	
	UFUNCTION(BlueprintCallable)
	void CheckResourceAvailability(bool ResourceAvailable);

};
