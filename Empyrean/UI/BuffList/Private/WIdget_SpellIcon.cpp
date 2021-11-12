// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SpellIcon.h"

#include "StatusBase.h"
#include "Kismet/KismetTextLibrary.h"

void UWidget_SpellIcon::InitializeIcon(UStatusBase* Status, UTexture2D* Icon, float Duration)
{
	AbilityIcon->SetBrushFromTexture(Icon);
	CurrentStacks = Status->CurrentStacks;
	UpdateStacksDisplay(CurrentStacks);

	Status->OnStacksUpdated.AddDynamic(this, &UWidget_SpellIcon::StacksUpdated);
	
	if (Duration > 0)
		PlayRadialSwipe(0, Duration);
}

void UWidget_SpellIcon::StacksUpdated(AC_Character* Character, UStatusBase* Status, int Stacks)
{
	UpdateStacksDisplay(Stacks);
}

void UWidget_SpellIcon::UpdateStacksDisplay(int Stacks, bool Force)
{	
	Stacks = Stacks == -1 ? CurrentStacks : Stacks;
	CurrentStacks = Stacks == -1 ? CurrentStacks : Stacks;
	if (Stacks > 1 || Force)
	{
		StacksText->SetText(UKismetTextLibrary::Conv_IntToText(Stacks));
		StacksText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
		StacksText->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_SpellIcon::CheckResourceAvailability(bool ResourceAvailable)
{
	if (ResourceAvailable)
		NoResourceOverlay->SetVisibility(ESlateVisibility::Collapsed);
	else
		NoResourceOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
}
