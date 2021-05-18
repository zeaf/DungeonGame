// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UnitFrameBase.h"

#include "AbilityCastingComponent.h"
#include "C_Character.h"
#include "HealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UUnitFrameBase::ClearTarget()
{
	SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(Target))
	{
		Target->Health->UpdateHealth.RemoveDynamic(this, &UUnitFrameBase::UpdateHealthEvent);
		Target->AbilityCastingComponent->OnCastStart.RemoveDynamic(this, &UUnitFrameBase::OnCastStart);
		Target->AbilityCastingComponent->OnCastEnd.RemoveDynamic(this, &UUnitFrameBase::OnCastEnd);
	}
	Target = nullptr;
}

void UUnitFrameBase::TargetUpdate(AC_Character* NewTarget)
{
	ClearTarget();
	SetVisibility(ESlateVisibility::HitTestInvisible);

	Target = NewTarget;
	Target->Health->UpdateHealth.AddDynamic(this, &UUnitFrameBase::UpdateHealthEvent);
	Target->AbilityCastingComponent->OnCastStart.AddDynamic(this, &UUnitFrameBase::OnCastStart);
	Target->AbilityCastingComponent->OnCastEnd.AddDynamic(this, &UUnitFrameBase::OnCastEnd);

	UpdateHealthEvent();

	if (Target->AbilityCastingComponent->IsCasting)
		StartLateCastBar(Target->AbilityCastingComponent->CurrentlyCastingAbility, Target->AbilityCastingComponent->CurrentAbilityCastStart, Target->AbilityCastingComponent->CurrentCastTime);
	else
		OnCastEnd();
}

void UUnitFrameBase::UpdateHealthEvent()
{
	UpdateWidgetHealthValues(
		UKismetTextLibrary::Conv_FloatToText(GetHPPct() * 100., HalfToEven, false, true, 1, 3, 1, 1), 
		GetCurrentHPText(), 
		GetHPPct());
}

void UUnitFrameBase::UpdateWidgetHealthValues_Implementation(const FText& HPPercent, const FText& HP, float HPBarPercent)
{
	HealthBar->SetPercent(HPBarPercent);
	HPPct->SetText(HPPercent);
	CurrentHP->SetText(HP);
}

FText UUnitFrameBase::GetCurrentHPText()
{
	return UKismetTextLibrary::Conv_FloatToText(Target->Health->CurrentHealth, HalfToEven, false, true, 1, 10, 0, 0);
}

float UUnitFrameBase::GetHPPct()
{
	return Target->Health->CurrentHealth / Target->Health->MaxHealth.CurrentValue;
}
