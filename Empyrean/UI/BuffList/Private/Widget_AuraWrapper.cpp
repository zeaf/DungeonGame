// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_AuraWrapper.h"

#include "StatusBase.h"
#include "Widget_AuraTooltip.h"
#include "Widget_SpellIcon.h"

void UWidget_AuraWrapper::InitializeAura(UStatusBase* Status, UTexture2D* StatusIcon, float Duration)
{
	Icon->InitializeIcon(Status, StatusIcon, Duration);
	Tooltip->InitializeTooltip(Status);
}
