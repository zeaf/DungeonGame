// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EmpyreanMenuGameMode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EMPYREAN_API AEmpyreanMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AEmpyreanMenuGameMode();


protected:
	UFUNCTION(BlueprintCallable)
	bool CreateHostBeacon();

	class AEmpyreanOnlineBeaconHostObject* HostObject;
};
