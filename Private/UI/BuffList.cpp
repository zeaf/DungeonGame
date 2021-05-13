// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BuffList.h"
#include "UI/Widget_AuraWrapper.h"
#include "Components/UniformGridPanel.h"
#include "UI/Widget_SpellIcon.h"

void UBuffList::GetRowAndColumn(int Index, int& Row, int& Column)
{
	Row = NumberOfRows - Index / NumberOfColumns - 1;
	Column = Index % NumberOfColumns;
}

void UBuffList::RemoveAura(int ID)
{
	//if (CurrentAuras > NumberOfColumns)
	//{
	//	auto temp = CreateWidget<UWidget_AuraWrapper>(this);
	//	temp->SetVisibility(ESlateVisibility::Hidden);
	//	AuraList->AddChildToUniformGrid(temp, 0, 0);
	//}
	AuraList->RemoveChild(AuraMap[ID]);
	
	TArray<UWidget_AuraWrapper*> PreviousAuras;
	auto Auras = AuraList->GetAllChildren();
	AuraList->ClearChildren();
	for (auto Aura : Auras)
	{
		//Aura->Visibility == ESlateVisibility::HitTestInvisible;
		if (Aura->Visibility != ESlateVisibility::Hidden)
			PreviousAuras.Add(Cast<UWidget_AuraWrapper>(Aura));
	}
	
	
	int Counter = 0;
	for (auto It : PreviousAuras)
	{
		int Row, Column;
		GetRowAndColumn(Counter, Row, Column);
		//UE_LOG(LogTemp, Warning, TEXT("%d %d"), Row, Column);
		AuraList->AddChildToUniformGrid(It, Row, Column);
		It->Icon->UpdateStacksDisplay(-1);
		Counter++;
	}
	//	int Row, Column;
	//	GetRowAndColumn(i, Row, Column);
	//	AuraList->AddChildToUniformGrid(Auras[i], Row, Column);
	//}

	for (int i = CurrentAuras % NumberOfColumns; i < NumberOfColumns; ++i)
	{
		auto temp = CreateWidget<UWidget_AuraWrapper>(this);
		temp->SetVisibility(ESlateVisibility::Hidden);
		AuraList->AddChildToUniformGrid(temp, NumberOfRows - CurrentAuras / NumberOfColumns - 1, i);
	}
}

void UBuffList::InitializeAuraList(AC_Character* Target)
{
	AuraList->ClearChildren();
	Super::InitializeAuraList(Target);
}

void UBuffList::AddNewAura(UWidget_AuraWrapper* NewAura)
{
	if (CurrentAuras > NumberOfColumns)
	{
		auto temp = CreateWidget<UWidget_AuraWrapper>(this);
		temp->SetVisibility(ESlateVisibility::Hidden);
		AuraList->AddChildToUniformGrid(temp, 0, 0);
	}
	
	int Row, Column;
	GetRowAndColumn(CurrentAuras, Row, Column);
	AuraList->AddChildToUniformGrid(NewAura, Row, Column);

	for (int i = CurrentAuras % NumberOfColumns; i < NumberOfColumns; ++i)
	{
		//GetRowAndColumn(i, Row, Column);
		auto temp = CreateWidget<UWidget_AuraWrapper>(this);
		temp->SetVisibility(ESlateVisibility::Hidden);
		AuraList->AddChildToUniformGrid(temp, Row, i);
	}
}
