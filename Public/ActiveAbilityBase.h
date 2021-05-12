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
	Interrupted,
	None
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMPYREAN_API UActiveAbilityBase : public UAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly)
	UAbilityCastingComponent* AbilityCastingComponent;
	
	UPROPERTY()
	FTimerHandle CooldownTimer;
	
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		bool CanCastWhileMoving;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		bool CanCastWhileCasting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		bool IsChanneled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		bool AffectedByCDR = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, AdvancedDisplay)
		bool CooldownReady = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		float Cost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		float CastTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Ability)
		float Cooldown = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Ability)
		float GCD = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, AdvancedDisplay)
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
	void ServerOnBeginCast(AbilityCastResult& CastResult);
	virtual void ServerOnBeginCast_Implementation(AbilityCastResult& CastResult) { CastResult = AbilityCastResult::Successful; }

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "Start Cooldown"))
	void BPServerStartCooldown(const float Duration, const bool IsGCD);
	virtual void BPServerStartCooldown_Implementation(const float Duration, const bool IsGCD);

	UFUNCTION(BlueprintNativeEvent)
	void ClientCooldown(const float Duration, const bool IsGCD);
	virtual void ClientCooldown_Implementation(const float Duration, const bool IsGCD) {};

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSuccessfulCastSequence();
	void ServerSuccessfulCastSequence_Implementation();

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = SuccessfulCastSequence))
	void BPServerSuccessfulCastSequence();
	virtual void BPServerSuccessfulCastSequence_Implementation();
	
	void ResetCooldown();

	UFUNCTION(BlueprintNativeEvent)
	UPARAM(DisplayName = "Can Cast") bool CastConditions();
	bool CastConditions_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	UPARAM(DisplayName = "ResourceAvailable") bool CheckResourceAvailability();
	UPARAM(DisplayName = "ResourceAvailable") virtual bool CheckResourceAvailability_Implementation();

	bool CheckCastableWhileMoving();

	float GetCDAfterCdr(const float CD);
	
	UFUNCTION(BlueprintNativeEvent)
	void StartGCD(const float Time);
	virtual void StartGCD_Implementation(const float Time);
};
