// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_StatusList.h"
#include "DebuffList.generated.h"

/**
 * 
 */
UCLASS()
class EMPYREAN_API UDebuffList : public UWidget_StatusList
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* AuraList;
	
public:
	virtual void InitializeAuraList(AC_Character* Target) override;

	virtual void AddNewAura(UWidget_AuraWrapper* NewAura) override;
};
