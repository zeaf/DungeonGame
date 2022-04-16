// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_Character.h"
#include "Containers/Map.h"
#include "HealthComponent.h"
#include "Components/ActorComponent.h"
#include "ThreatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UThreatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UThreatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Threat")
	AC_Character* Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Threat")
	UHealthComponent* HealthComp;

	UFUNCTION(BlueprintCallable)
		void AddThreat(AC_Character* DamageDealer, const FDamageOutcome Outcome);

	UFUNCTION(BlueprintCallable)
		void RemoveFromThreatMap(AC_Character* Character);

	TMap<AC_Character*, float> ThreatMap;

	AC_Character* GetHighestThreatActor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
