// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_AuraTooltip.h"

#include "StatusBase.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UWidget_AuraTooltip::InitializeTooltip(UStatusBase* Status)
{
	Name->SetText(UKismetTextLibrary::Conv_NameToText(Status->Name));
	Description->SetText(Status->Description);
}
