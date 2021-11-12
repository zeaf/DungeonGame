// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterDataRowNew.h"

#include "AbilityCastingComponent.h"
#include "C_Character.h"
#include "HealthComponent.h"
#include "NPCDataTableRow.h"
#include "UpdateCharacterValuesNew.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetTextLibrary.h"

void UCharacterDataRowNew::InitializeRow_Implementation(AC_Character* Character, UDataTable* Table, FName Row)
{
	if (!Character) return;
	CharacterRef = Character;
	Data = Table;
	RowName = Row;
	Name->SetText(UKismetTextLibrary::Conv_StringToText(Character->GetName()));
	//SetPropertyObject(CharacterDetails, Character);
	//SetPropertyObject(HealthDetails, Character->Health);
	//SetPropertyObject(AbilityDetails, Character->AbilityCastingComponent);
}

//void UCharacterDataRow::SetPropertyObject(UPropertyViewBase* Property, UObject* Character)
//{
//	//if (Property)
//	//	Property->SetObject(Character);
//}

void UCharacterDataRowNew::WriteToDataTable()
{
	if (Data)
	{
		auto RowData = Data->FindRow<FNPCCharacterDataTableRow>(RowName, "");
		RowData->MaxHealth = CharacterRef->Health->MaxHealth.CurrentValue;
		RowData->Minhealth = CharacterRef->Health->MinHealth;
		RowData->ActiveAbilities = CharacterRef->AbilityCastingComponent->ActiveAbilitiesNew;
		if (RowData->PassiveAbilities.IsValidIndex(0))
			RowData->PassiveAbilities[0] = CharacterRef->AbilityCastingComponent->PassiveAbilityReference;
		else
			RowData->PassiveAbilities.Add(CharacterRef->AbilityCastingComponent->PassiveAbilityReference);
		
	}
}
