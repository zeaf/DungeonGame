// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "CharacterDataRowNew.generated.h"

#if WITH_EDITOR

class UDataTable;
class UTextBlock;
class AC_Character;
/**
 * 
 */


UCLASS()
class UCharacterDataRowNew : public UEditorUtilityWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;

//#if WITH_EDITOR
//	class UPropertyViewBase*
//	UPROPERTY(meta = (BindWidget))
//		UPropertyViewBase* CharacterDetails;
//
//	UPROPERTY(meta = (BindWidget))
//		UPropertyViewBase* HealthDetails;
//
//	UPROPERTY(meta = (BindWidget))
//		UPropertyViewBase* AbilityDetails;
//#endif
	
	UPROPERTY()
	AC_Character* CharacterRef;


	UPROPERTY()
	FName RowName;

	UPROPERTY()
	UDataTable* Data;
public:
	
	UFUNCTION(BlueprintNativeEvent)
	void InitializeRow(AC_Character* Character, UDataTable* Table, FName Row);
	void InitializeRow_Implementation(AC_Character* Character, UDataTable* Table, FName Row);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPropertyObject(UWidget* Property, UObject* Character);
	void SetPropertyObject_Implementation(UWidget* Property, UObject* Character) {}
	
	UFUNCTION(BlueprintCallable)
	void WriteToDataTable();
};

#endif

