// Fill out your copyright notice in the Description page of Project Settings.


#include "EmpyreanOnlineBeaconHostObject.h"

#include "EmpyreanOnlineBeaconClient.h"

AEmpyreanOnlineBeaconHostObject::AEmpyreanOnlineBeaconHostObject()
{
	ClientBeaconActorClass = AEmpyreanOnlineBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();
}

void AEmpyreanOnlineBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor,
	UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);

	if (NewClientActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Client connected"));	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client invalid"));
	}
}
