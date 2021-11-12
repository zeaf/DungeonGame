// Fill out your copyright notice in the Description page of Project Settings.


#include "EmpyreanMenuGameMode.h"

#include "EmpyreanOnlineBeaconClient.h"
#include "EmpyreanOnlineBeaconHostObject.h"
#include "OnlineBeaconHost.h"
#include "OnlineSubsystem.h"

AEmpyreanMenuGameMode::AEmpyreanMenuGameMode()
{

}

bool AEmpyreanMenuGameMode::CreateHostBeacon()
{
	AOnlineBeaconHost* Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass());
	if (Host)
	{
		UE_LOG(LogTemp, Warning, TEXT("Host beacon created"));
		if (Host->InitHost())
		{
			Host->PauseBeaconRequests(false);
			UE_LOG(LogTemp, Warning, TEXT("Host Initialized"));
			HostObject = GetWorld()->SpawnActor<AEmpyreanOnlineBeaconHostObject>(AEmpyreanOnlineBeaconHostObject::StaticClass());
			if (HostObject)
			{
				UE_LOG(LogTemp, Warning, TEXT("Host object created"));
				Host->RegisterHost(HostObject);
				return true;
			}
		}
	}
	return false;
}
