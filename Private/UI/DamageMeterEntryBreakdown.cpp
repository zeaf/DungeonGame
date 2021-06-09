// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageMeterEntryBreakdown.h"

#include "AbilityBase.h"
#include "WrapBoxNoEditor.h"
#include "UI/DamageMeterBreakdownEntry.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"

void UDamageMeterEntryBreakdown::ShowBreakdown()
{	
	Entries.ValueSort([](const UDamageMeterBreakdownEntry& A, const UDamageMeterBreakdownEntry& B) {
		return A.CurrentValue > B.CurrentValue; });

	int i = 0;
	for (auto& It : Entries)
	{
		It.Value->UpdateBreakdownEntry(CurrentMax, TotalDamage);
		if (EntryList->GetChildIndex(It.Value) != i)
			EntryList->ShiftChildFixed(i, It.Value);
		++i;
	}
}

void UDamageMeterEntryBreakdown::UpdateEntry(UAbilityBase* Ability, float Amount, float TotalCharacterDamage)
{
	TotalDamage = TotalCharacterDamage;
	if (!IsValid(Ability)) return;
	if (Ability->AbilityName == NAME_None) return;
	if (Entries.Find(Ability->GetUniqueID()))
	{
		const float NewAmount = Entries[Ability->GetUniqueID()]->CurrentValue + Amount;
		Entries[Ability->GetUniqueID()]->CurrentValue = NewAmount;

		CurrentMax = NewAmount >= CurrentMax ? NewAmount : CurrentMax;
		EntryValues.Add(Ability->GetUniqueID(), NewAmount);
		Entries[Ability->GetUniqueID()]->UpdateBreakdownEntry(CurrentMax, TotalDamage);
	}
	else
	{
		UDamageMeterBreakdownEntry* NewEntry = CreateWidget<UDamageMeterBreakdownEntry>(this, EntryClass);
		Entries.Emplace(Ability->GetUniqueID(), NewEntry);

		EntryValues.Add(Ability->GetUniqueID(), Amount);
		CurrentMax = Amount >= CurrentMax ? Amount : CurrentMax;
		NewEntry->InitializeEntry(Ability, Amount, CurrentMax, TotalDamage);
		EntryList->AddChild(NewEntry);
	}

	if (Visibility == ESlateVisibility::HitTestInvisible)
		ShowBreakdown();
}
