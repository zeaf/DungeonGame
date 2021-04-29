// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ActiveAbilityBase.h"
#include "Components/ActorComponent.h"
#include "AbilityCastingComponent.generated.h"


class AC_Character;
class UActiveAbilityBase;
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

private:
	AC_Character* Character;
	UPROPERTY(EditDefaultsOnly)
	float SpellQueueWindow = 0.5f;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileMoving;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileCasting;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UActiveAbilityBase* CurrentlyCastingAbility;


	UPROPERTY()
	UActiveAbilityBase* QueuedAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsCasting;

	UPROPERTY(BlueprintReadWrite)
		FTimerHandle CastingTimer;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, SimpleDisplay, Category = "Abilities", Meta = (DisplayName = "Active Abilities", ExposeFunctionCategories = "Abilities", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UActiveAbilityBase>> ActiveAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, SimpleDisplay, Category = "Abilities", Meta = (DisplayName = "Passive Ability", ExposeFunctionCategories = "Abilities", AllowPrivateAccess = "true"))
	TSubclassOf<UAbilityBase> PassiveAbility;

	UPROPERTY(BlueprintReadOnly)
	TArray<UActiveAbilityBase*> Abilities;

	void InitializeAbilities();

	UFUNCTION(Server, Reliable)
	void ServerCastTime();
	void ServerCastTime_Implementation();

	UFUNCTION()
	void SuccessfulCastSequence();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerAttemptToCast(UActiveAbilityBase* Ability);
	void ServerAttemptToCast_Implementation(UActiveAbilityBase* Ability);

	UFUNCTION(Client, Reliable)
	void ClientCastbar(const UActiveAbilityBase* Ability);
	void ClientCastbar_Implementation(const UActiveAbilityBase* Ability);
	
	void TriggerGCD(const float Time);

	UFUNCTION()
	void CastQueuedAbility();
};

inline void UAbilityCastingComponent::CastQueuedAbility()
{
	if(QueuedAbility)
	{
		CurrentlyCastingAbility->OnCastSuccess.Remove(this, FName("CastQueuedAbility"));
		ServerAttemptToCast(QueuedAbility);
		QueuedAbility = nullptr;
	}
}
