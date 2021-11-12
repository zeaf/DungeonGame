// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageMeterBreakdownEntry.h"

#include "AbilityBase.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UDamageMeterBreakdownEntry::UpdateBreakdownEntry(float CurrentMax, float TotalDamage)
{
	BarFillPercentage = CurrentValue / CurrentMax;
	Percentage->SetText(UKismetTextLibrary::Conv_FloatToText((CurrentValue / TotalDamage) * 100., HalfFromZero,
		false, true, 1, 3, 1, 1));
	Value->SetText(UKismetTextLibrary::Conv_FloatToText(CurrentValue, HalfFromZero,
		false, true, 1, 15, 0, 0));
}

void UDamageMeterBreakdownEntry::InitializeEntry(UAbilityBase* Ability, float Amount, float CurrentMax, float TotalDamage)
{
	Icon->SetBrushFromTexture(Ability->Icon);
	CurrentValue = Amount;
	Name->SetText(UKismetTextLibrary::Conv_NameToText(Ability->AbilityName));
	UpdateBreakdownEntry(CurrentMax, TotalDamage);
}

void UDamageMeterBreakdownEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Bar->SetPercent(FMath::FInterpTo(Bar->Percent, BarFillPercentage, InDeltaTime, 7));
}
