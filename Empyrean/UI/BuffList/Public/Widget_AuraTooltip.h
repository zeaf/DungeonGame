// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_AuraTooltip.generated.h"

class UStatusBase;
class UTextBlock;

UCLASS()
class EMPYREAN_API UWidget_AuraTooltip : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;

public:
	void InitializeTooltip(UStatusBase* Status);
};
