// Fill out your copyright notice in the Description page of Project Settings.


#include "WrapBoxNoEditor.h"

void UWrapBoxNoEditor::ShiftChildFixed(int32 Index, UWidget* Child)
{
	int32 CurrentIndex = GetChildIndex(Child);
	Slots.RemoveAt(CurrentIndex);
	Slots.Insert(Child->Slot, FMath::Clamp(Index, 0, Slots.Num()));
}
