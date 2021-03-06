// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ActiveAbilityBase.h"
#include "Components/ActorComponent.h"
#include "AbilityCastingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCastStart, UActiveAbilityBase*, Ability, float, CastTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCastEnd);
class AC_Character;
class UActiveAbilityBase;
class UAbilityBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMPYREAN_API UAbilityCastingComponent : public UActorComponent
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

	UPROPERTY(BlueprintAssignable, AdvancedDisplay)
		FOnCastStart OnCastStart;

	UPROPERTY(BlueprintAssignable, AdvancedDisplay)
		FOnCastEnd OnCastEnd;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileMoving;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileCasting;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UActiveAbilityBase* CurrentlyCastingAbility;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float CurrentCastTime;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FDateTime CurrentAbilityCastStart;

	UPROPERTY()
	UActiveAbilityBase* QueuedAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsCasting;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle CastingTimer;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle GlobalCooldownTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, SimpleDisplay, Category = "Abilities", Meta = (DisplayName = "Active Abilities", ExposeFunctionCategories = "Abilities", AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UActiveAbilityBase>> ActiveAbilitiesNew;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, SimpleDisplay, Category = "Abilities", Meta = (DisplayName = "Passive Ability", ExposeFunctionCategories = "Abilities", AllowPrivateAccess = "true"))
	TSubclassOf<UAbilityBase> PassiveAbilityReference;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UActiveAbilityBase*> Abilities;

	UPROPERTY(BlueprintReadOnly, Replicated)
	UAbilityBase* PassiveAbility;
	
	void InitializeAbilities();

	UFUNCTION(Server, Reliable)
	void ServerCastTime(float CastTime);
	void ServerCastTime_Implementation(float CastTime);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetCurrentCastParameters(UActiveAbilityBase* Ability, float CastTime);
	void MulticastSetCurrentCastParameters_Implementation(UActiveAbilityBase* Ability, float CastTime);
	UFUNCTION()
	void SuccessfulCastSequence();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerAttemptToCast(UActiveAbilityBase* Ability);
	void ServerAttemptToCast_Implementation(UActiveAbilityBase* Ability);

	bool InGCD;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerInterruptCast();
	void ServerInterruptCast_Implementation();

	UFUNCTION(Client, Reliable)
	void ClientCastbar(const UActiveAbilityBase* Ability, const float CastTime);
	void ClientCastbar_Implementation(const UActiveAbilityBase* Ability, const float CastTime);
	
	void TriggerGCD(const float Time, const UActiveAbilityBase* TriggeringAbility);
		
	void GCDEnd();
	
	float GetCastTimeAfterHaste(const float CastTime);

	UFUNCTION()
	void CastQueuedAbility();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPARAM(meta = (DisplayName = CanCast)) bool CheckAbilityCastConditions(const int AbilityNumber, UActiveAbilityBase*& Ability);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
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
