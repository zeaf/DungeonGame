// Fill out your copyright notice in the Description page of Project Settings.


#include "EmpyreanOnlineBeaconClient.h"

AEmpyreanOnlineBeaconClient::AEmpyreanOnlineBeaconClient()
{
}

bool AEmpyreanOnlineBeaconClient::ConnectToServer(const FString& Address)
{
	FURL Destination = FURL(nullptr, *Address, TRAVEL_Absolute);

	Destination.Port = 7787;

	return InitClient(Destination);
}

void AEmpyreanOnlineBeaconClient::OnFailure()
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT FAILED TO CONNECT TO HOST BEACON"));
}

void AEmpyreanOnlineBeaconClient::OnConnected()
{
	UE_LOG(LogTemp, Warning, TEXT("CLIENT CONNECTED SUCCESSFULLY TO HOST BEACON"));
}
