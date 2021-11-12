// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_AuraWrapper.generated.h"

class UStatusBase;
class UWidget_AuraTooltip;
class UWidget_SpellIcon;
class UImage;
/**
 * 
 */
UCLASS()
class EMPYREAN_API UWidget_AuraWrapper : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Border;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWidget_AuraTooltip* Tooltip;

	UPROPERTY(meta = (BindWidget))
	UWidget_SpellIcon* Icon;

	UFUNCTION(BlueprintCallable)
	void InitializeAura(UStatusBase* Status, UTexture2D* StatusIcon, float Duration);
};

