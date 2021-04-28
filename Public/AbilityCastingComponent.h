// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityCastingComponent.generated.h"


class UAbilityBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELENAPLAYGROUND_API UAbilityCastingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityCastingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileMoving;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileCasting;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UAbilityBase* CurrentlyCastingAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsCasting;

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle CastingTimer;

	UFUNCTION(Server, Reliable)
		void ServerCastTime();
};
