// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

/**
 * 
 */

class HELENAPLAYGROUND_API SpawnDeferred
{
	public:
		SpawnDeferred();
		~SpawnDeferred();

		UFUNCTION(BlueprintCallable, Category = "Actor");
		APawn SpawnActorDeferred(FVector location, AActor actorClass, APawn instigator, FVector forward);
};
