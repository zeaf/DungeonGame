// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "C_Character.h"
#include "EditorUtilityWidget.h"
#include "Engine/DataTable.h"

#include "UpdateCharacterValuesNew.generated.h"

class UCharacterDataRowNew;
class UActiveAbilityBase;
class UDataTable;
class UButton;
class UScrollBox;
/**
 *
 * 
 */

UCLASS(Blueprintable)
class UUpdateCharacterValuesNew : public UEditorUtilityWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* List;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* Data;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UCharacterDataRowNew> RowTemplate;
	
	UFUNCTION(BlueprintCallable)
	void RefreshTable();

	UPROPERTY()
	TMap<TSubclassOf<AC_Character>, UCharacterDataRowNew*> Entries;
	
};


