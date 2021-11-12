// Fill out your copyright notice in the Description page of Project Settings.


#include "EmpyreanGameInstance.h"


void UEmpyreanGameInstance::OnSessionInvite(const FUniqueNetId& UserId, const FUniqueNetId& FromId,
	const FString& AppId, const FOnlineSessionSearchResult& SessionToJoin)
{
	UE_LOG(LogTemp, Warning, TEXT("INVITE RECEIVED"));
}

void UEmpyreanGameInstance::Init()
{
	Super::Init();

	IOnlineSessionPtr Session = IOnlineSubsystem::Get()->GetSessionInterface();

	if (Session)
	{
		UE_LOG(LogTemp, Warning, TEXT("Online Interface Exists"));

		FOnSessionInviteReceivedDelegate OnSessionInviteReceivedDelegate = FOnSessionInviteReceivedDelegate::CreateUObject(this, &UEmpyreanGameInstance::OnSessionInvite);
	}
	
}

void UEmpyreanGameInstance::GetIPFromSession(const FBlueprintSessionResult& SessionToJoin)
{
	FString URL = FString();
	IOnlineSessionPtr Session = IOnlineSubsystem::Get()->GetSessionInterface();

	if (Session)
	{
		FNamedOnlineSession* NamedOnlineSession = Session->GetNamedSession(GameSessionName);
		if (NamedOnlineSession)
		{
			if (IsDedicatedServerInstance()) URL.Append(TEXT("steam."));
			URL.Append(NamedOnlineSession->SessionInfo->GetSessionId().ToString());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("IP"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *URL);
}
