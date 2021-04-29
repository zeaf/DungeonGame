// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "ActiveAbilityBase.generated.h"


class UAbilityCastingComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastSuccessDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastFailedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastInterruptedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCastStoppedDelegate);

UENUM()
enum class AbilityCastResult : uint8
{
	Successful,
	Failed,
	Stopped,
	Interrupted
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELENAPLAYGROUND_API UActiveAbilityBase : public UAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly)
	UAbilityCastingComponent* AbilityCastingComponent;
	
	UPROPERTY()
	FTimerHandle CooldownTimer;
	
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileMoving;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CanCastWhileCasting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool IsChanneled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool CooldownReady;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Cost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CastTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Cooldown = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float GCD = 1.5f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float ElapsedCD;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FCastSuccessDelegate OnCastSuccess;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FCastFailedDelegate OnCastFailed;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FCastInterruptedDelegate OnCastInterrupted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FCastInterruptedDelegate OnCastStopped;
	
	UFUNCTION(BlueprintNativeEvent)
	void CastSuccess();
	virtual void CastSuccess_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void ServerOnBeginCast();
	virtual void ServerOnBeginCast_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void ServerOnFinishedCast();
	virtual void ServerOnFinishedCast_Implementation() {}

	virtual void BPRemoveResource_Implementation() override;	

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAbilityCast();
	void MulticastAbilityCast_Implementation() { BPMulticastAbilityCast(); }

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = MulticastAbilityCast))
	void BPMulticastAbilityCast();
	virtual void BPMulticastAbilityCast_Implementation() {}

	UFUNCTION(Server, Reliable)
	void ServerAbilityEndCast(AbilityCastResult CastResult);
	void ServerAbilityEndCast_Implementation(AbilityCastResult CastResult) { BPServerAbilityEndCast(CastResult); }

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = EndCast))
	void BPServerAbilityEndCast(AbilityCastResult CastResult);
	virtual void BPServerAbilityEndCast_Implementation(AbilityCastResult CastResult);

	UFUNCTION(Server, Reliable)
	void ServerStartCooldown(const float Duration, const bool IsGCD);
	void ServerStartCooldown_Implementation(const float Duration, const bool IsGCD);

	UFUNCTION(BlueprintNativeEvent)
	void ClientCooldown(const float Duration, const bool IsGCD);
	virtual void ClientCooldown_Implementation(const float Duration, const bool IsGCD) {};

	void ResetCooldown();

	UFUNCTION(BlueprintNativeEvent)
	UPARAM(DisplayName = "Can Cast") bool CastConditions();
	bool CastConditions_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	UPARAM(DisplayName = "Resource Available") bool CheckResourceAvailability();
	bool CheckResourceAvailability_Implementation();

	bool CheckCastableWhileMoving();

	void StartGCD(const float Time);
};
