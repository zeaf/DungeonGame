// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonGameStatics.generated.h"

UENUM(BlueprintType)
enum class Classes : uint8
{
	None				UMETA(DisplayName = "None"),
	Berserker				UMETA(DisplayName = "Berserker"),
	Astromancer				UMETA(DisplayName = "Astromancer")
};

UCLASS(BlueprintType, Blueprintable)
class EMPYREAN_API UDungeonGameStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<Classes, FColor> ClassColors;

	UFUNCTION(BlueprintCallable, Category = DungeonGame)
	static FColor GetClassColor(Classes Class);
};
