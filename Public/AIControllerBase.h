// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/UObjectGlobals.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

class AC_Character;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HELENAPLAYGROUND_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		AC_Character* ControlledPawn;
	
	UPROPERTY()
		AC_Character* CurrentTarget;
	
	UPROPERTY()
		class UThreatComponent* ThreatComponent;
	
public:
	AAIControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaSeconds) override;	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		AC_Character* GetCurrentTarget() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ThreatTarget)
		bool UpdateThreatTarget = true;
	
	UFUNCTION(BlueprintCallable)
		void SetCurrentTarget(AC_Character* Target);

	UFUNCTION(BlueprintNativeEvent)
		void OnTargetChanged();
	virtual void OnTargetChanged_Implementation() {};

private:
	AC_Character* GetThreatTarget() const;
};
