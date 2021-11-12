// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WrapBox.h"
#include "WrapBoxNoEditor.generated.h"

/**
 * 
 */
UCLASS()
class EMPYREAN_API UWrapBoxNoEditor : public UWrapBox
{
	GENERATED_BODY()

public:
	void ShiftChildFixed(int32 Index, UWidget* Child);
};
