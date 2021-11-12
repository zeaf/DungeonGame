// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_StatusList.h"

#include "Widget_AuraWrapper.h"
#include "C_Character.h"
#include "StatusComponent.h"
#include "Widget_SpellIcon.h"

bool UWidget_StatusList::IsAuraIncluded(UStatusBase* Status)
{
	switch (IncludedAuras)
	{
		case AuraListIncludedAuras::Both: 
			return true;
		case AuraListIncludedAuras::OwnOnly: 
			if (Status->Instigator == Status->TargetActor)
				return true;
			return false;
		case AuraListIncludedAuras::ExternalOnly: 
			if (Status->Instigator != Status->TargetActor)
				return true;
			return false;
	}
	return true;
}

void UWidget_StatusList::OnStatusApplied(AC_Character* Target, UStatusBase* Status, int ID, UTexture2D* Icon, float Duration)
{
	if ((BuffContainer && !Status->IsDebuff) || (!BuffContainer && Status->IsDebuff))
	{
		if (!IsAuraIncluded(Status)) return;

		//UE_LOG(LogTemp, Warning, TEXT("%s, %s, %d"), *Target->GetName(), *Status->GetName(), ID);
		
		UWidget_AuraWrapper* NewAuraWidget = CreateWidget<UWidget_AuraWrapper>(this, AuraWrapperClass);
		NewAuraWidget->InitializeAura(Status,Icon, Duration);
		AddNewAura(NewAuraWidget);
		AuraMap.Add(ID, NewAuraWidget);
		CurrentAuras++;
	}

}

void UWidget_StatusList::OnStatusRemoved(AC_Character* Target, int ID)
{
	if (AuraMap.Find(ID))
	{
		AuraMap[ID]->RemoveFromParent();
		RemoveAura(ID);
		AuraMap.Remove(ID);
		CurrentAuras--;
	}
}

void UWidget_StatusList::OnStatusRefreshed(AC_Character* Target, UStatusBase* Status, int ID, int Stacks)
{
	if (AuraMap.Find(ID))
	{
		if (Status->CanBeRefreshed)
			AuraMap[ID]->Icon->PlayRadialSwipe(0, Status->Duration);
		
		AuraMap[ID]->Icon->UpdateStacksDisplay(Stacks);
	}
}


void UWidget_StatusList::InitializeAuraList(AC_Character* Target)
{
	Character = Target;
	
	CurrentAuras = 0;
	
	AC_Character* Temp = Character.Get();
	
	Temp->StatusComponent->OnStatusApplied.AddDynamic(this, &UWidget_StatusList::OnStatusApplied);

	Temp->StatusComponent->OnStatusRefreshed.AddDynamic(this, &UWidget_StatusList::OnStatusRefreshed);

	Temp->StatusComponent->OnStatusRemoved.AddDynamic(this, &UWidget_StatusList::OnStatusRemoved);

}
