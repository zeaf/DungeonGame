// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PortraitCaptureComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMPYREAN_API UPortraitCaptureComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:
	UPortraitCaptureComponent();
};
