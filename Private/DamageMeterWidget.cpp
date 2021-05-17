// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageMeterWidget.h"

#include "DamageMeterEntry.h"
#include "Components/ScrollBox.h"

void UDamageMeterWidget::UpdateEntry(AC_Character* Character, const float AmountDealt, UAbilityBase* Ability)
{
	if (EntriesMap.Find(Character))
	{
		float NewValue = ValuesMap.Add(Character, ValuesMap[Character] + AmountDealt);
		CurrentMax = NewValue >= CurrentMax ? NewValue : CurrentMax;
		EntriesMap[Character]->UpdateValue(AmountDealt, CurrentMax, Ability);
	}
	else
	{
		UDamageMeterEntry* NewEntry = CreateEntryWidget();
		EntriesMap.Emplace(Character, NewEntry);
		float NewValue = ValuesMap.Emplace(Character, AmountDealt);
		CurrentMax = NewValue >= CurrentMax ? NewValue : CurrentMax;
		NewEntry->InitializeDamageMeterEntry(Character, AmountDealt, CurrentMax, Ability);

		MeterEntries->AddChild(NewEntry);	
	}
	
	MeterEntries->ClearChildren();
	
	EntriesMap.ValueSort([](const UDamageMeterEntry& A, const UDamageMeterEntry& B) {
		return A.TotalAmountDealt > B.TotalAmountDealt; });

	for (auto& Entry : EntriesMap)
	{
		Entry.Value->UpdateBarPercent(CurrentMax);
		MeterEntries->AddChild(Entry.Value);
	}
}

UDamageMeterEntry* UDamageMeterWidget::CreateEntryWidget_Implementation()
{
	return CreateWidget<UDamageMeterEntry>(this, EntryClass);
}
