// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget_AuraWrapper.h"

#include "StatusBase.h"
#include "UI/Widget_AuraTooltip.h"
#include "UI/Widget_SpellIcon.h"

void UWidget_AuraWrapper::InitializeAura(UStatusBase* Status, UTexture2D* StatusIcon, float Duration)
{
	Icon->InitializeIcon(Status, StatusIcon, Duration);
	Tooltip->InitializeTooltip(Status);
}
