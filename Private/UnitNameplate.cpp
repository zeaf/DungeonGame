// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitNameplate.h"

#include "C_Character.h"
#include "HealthComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "UI/Castbar.h"
#include "UI/DebuffList.h"

void UUnitNameplate::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUnitNameplate::NativeConstruct()
{
	Super::NativeConstruct();

	Castbar->InitializeCastbar(Character);

	float C, M, MH, Pct;
	Character->Health->GetHealth(C, M, MH, Pct);
	SetHPPercentageText(Pct);

	Character->Health->UpdateHealth.AddDynamic(this, &UUnitNameplate::UpdateHealthEvent);

	Auras->InitializeAuraList(Character);
}

void UUnitNameplate::UpdateHealthEvent(float Current, float Max, float Percentage)
{
	SetHPPercentageText(Percentage);
	Healthbar->SetPercent(Percentage);
}

void UUnitNameplate::AddFloatingCombatText_Implementation(UUserWidget* FCTWidget, float Value, bool IsCrit, bool IsHealing, bool IsAbsorb)
{
	UCanvasPanelSlot* TextSlot = Canvas->AddChildToCanvas(FCTWidget);
	TextSlot->SetAlignment(FVector2D(0.5, 0.5));
	TextSlot->SetAnchors(FAnchors(0.5, 0.5, 0.5, 0.5));
	TextSlot->SetPosition(FVector2D(FMath::FRandRange(-75, 75), FMath::FRandRange(-20, -60)));
}

void UUnitNameplate::SetHPPercentageText(float Percentage)
{
	HPPercentage->SetText(UKismetTextLibrary::Conv_FloatToText(Percentage * 100, HalfFromZero,
		false, true, 1, 3, 0, 1));
}
