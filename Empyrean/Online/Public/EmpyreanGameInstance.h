// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "EmpyreanGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EMPYREAN_API UEmpyreanGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()


	//UFUNCTION(BlueprintNativeEvent)
	void OnSessionInvite(const FUniqueNetId& UserId, const FUniqueNetId& FromId, const FString& AppId, const FOnlineSessionSearchResult& SessionToJoin);
	//virtual void OnSessionInvite_Implementation(const FUniqueNetId& UserId, const FUniqueNetId& FromId, const FString& AppId, const FOnlineSessionSearchResult& SessionToJoin) {}
public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable)
		void GetIPFromSession(const FBlueprintSessionResult& SessionToJoin);
};
