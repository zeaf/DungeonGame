// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebuffList.h"
#include "UI/Widget_AuraWrapper.h"
#include "Components/WrapBox.h"

void UDebuffList::InitializeAuraList(AC_Character* Target)
{
	AuraList->ClearChildren();
	Super::InitializeAuraList(Target);
}

void UDebuffList::AddNewAura(UWidget_AuraWrapper* NewAura)
{
	AuraList->AddChildToWrapBox(NewAura);
}
