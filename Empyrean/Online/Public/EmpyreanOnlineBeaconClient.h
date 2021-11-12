// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "EmpyreanOnlineBeaconClient.generated.h"

/**
 * 
 */
UCLASS()
class EMPYREAN_API AEmpyreanOnlineBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()
public:
		AEmpyreanOnlineBeaconClient();

protected:
	UFUNCTION(BlueprintCallable)
		bool ConnectToServer(const FString& Address);

	virtual void OnFailure() override;
	virtual void OnConnected() override;
};
