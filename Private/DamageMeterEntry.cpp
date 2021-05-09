// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageMeterEntry.h"

#include <string>

#include "C_Character.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UDamageMeterEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateDynamicInformation(InDeltaTime);
}

void UDamageMeterEntry::UpdatePerSecondValue()
{
	float SecondsActive = FMath::RoundFromZero((FDateTime::UtcNow() - StartTime).GetTotalSeconds());
	PerSecondValue->SetText(UKismetTextLibrary::Conv_FloatToText(TotalAmountDealt / SecondsActive, HalfFromZero,
		false, true, 1, 15, 1, 1));
}

void UDamageMeterEntry::InitializeDamageMeterEntry(AC_Character* Instigator, const float AmountDealt, const float CurrentMax)
{
	Character = Instigator;
	NameBlock->SetText(FText::FromString(Instigator->GetName()));
	UpdateValue(AmountDealt, CurrentMax);
	UpdateBarPercent(CurrentMax);
	StartTime = FDateTime::UtcNow();
	GetWorld()->GetTimerManager().SetTimer(PerSecondUpdateTimer, this, &UDamageMeterEntry::UpdatePerSecondValue, 1.f, true);
}

void UDamageMeterEntry::UpdateValue(const float AmountDealt, const float CurrentMax)
{
	Amount->SetText(UKismetTextLibrary::Conv_FloatToText(AmountDealt, HalfFromZero,
		false, true, 1, 15, 1, 1));
	TotalAmountDealt = AmountDealt;
}

void UDamageMeterEntry::UpdateBarPercent(const float CurrentMax)
{
	//Bar->SetPercent(TotalAmountDealt / CurrentMax);
	BarFillPercent = TotalAmountDealt / CurrentMax;
}

void UDamageMeterEntry::UpdateDynamicInformation(const float InDeltaTime)
{
	Bar->SetPercent(FMath::FInterpTo(Bar->Percent, BarFillPercent, InDeltaTime, 5.f));
}
