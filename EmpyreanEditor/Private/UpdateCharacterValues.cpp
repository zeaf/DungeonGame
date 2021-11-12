// Fill out your copyright notice in the Description page of Project Settings.

#include "UpdateCharacterValuesNew.h"

#include "AbilityCastingComponent.h"
#include "C_Character.h"
#include "EditorUtilityLibrary.h"
#include "HealthComponent.h"
#include "Components/ScrollBox.h"
#include "Editor/EditorEngine.h"
#include "CharacterDataRowNew.h"
#include "NPCDataTableRow.h"
#include "Components/ScrollBoxSlot.h"
#include "Engine/DataTable.h"

void UUpdateCharacterValuesNew::NativeConstruct()
{
	Super::NativeConstruct();

	//RefreshTable();
}


void UUpdateCharacterValuesNew::RefreshTable()
{
	if (!Data) return;
	TArray<FNPCCharacterDataTableRow*> Rows;

	auto RowNames = Data->GetRowNames();
	
	Data->GetAllRows<FNPCCharacterDataTableRow>("Character Widget", Rows);

	for (auto Entry : Entries)
		Entry.Value->RemoveFromParent();
	
	Entries.Empty();
	
	for (auto R : RowNames)
	{
		auto Row = Data->FindRow<FNPCCharacterDataTableRow>(R, "");
		//<FNPCCharacterDataTableRow>();
		if (!Entries.Find(Row->Character))
		{
			if (!RowTemplate) return;

			AC_Character* CDO = Row->Character->GetDefaultObject<AC_Character>();
			if (!CDO) return;
			
			UCharacterDataRowNew* RowWidget = CreateWidget<UCharacterDataRowNew>(this, RowTemplate);

			if (!RowWidget) return;
			
			RowWidget->InitializeRow(CDO, Data, R);
			Entries.Emplace(Row->Character, RowWidget);
			auto WidgetSlot = Cast<UScrollBoxSlot>(List->AddChild(RowWidget));
			WidgetSlot->HorizontalAlignment = HAlign_Left;
		}
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Row->Character->GetName());
	}
}
