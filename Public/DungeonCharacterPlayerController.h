// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DungeonCharacterPlayerController.generated.h"

class UActiveAbilityBase;
/**
 * 
 */
UCLASS()
class HELENAPLAYGROUND_API ADungeonCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Castbar(const UActiveAbilityBase* Ability, float CastTime);
	virtual void Castbar_Implementation(const UActiveAbilityBase* Ability, float CastTime) {};
	
};
