// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget_StatusList.h"
#include "BuffList.generated.h"

class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class EMPYREAN_API UBuffList : public UWidget_StatusList
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetRowAndColumn(int Index, int& Row, int& Column);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUniformGridPanel* AuraList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumberOfRows = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumberOfColumns = 7;

	
	virtual void RemoveAura(int ID) override;
	
public:
	virtual void InitializeAuraList(AC_Character* Target) override;

	virtual void AddNewAura(UWidget_AuraWrapper* NewAura) override;

	
};
