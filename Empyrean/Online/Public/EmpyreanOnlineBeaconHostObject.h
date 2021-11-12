// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "EmpyreanOnlineBeaconHostObject.generated.h"

/**
 * 
 */
UCLASS()
class EMPYREAN_API AEmpyreanOnlineBeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
public:
		AEmpyreanOnlineBeaconHostObject();

	virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
};
