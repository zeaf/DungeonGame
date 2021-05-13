// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityBase.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


class AC_Character;
class UStatusBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnStatusApplied, AC_Character*, Target, UStatusBase*, Status, int, ID, UTexture2D*, Icon, float, Duration);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStatusRefreshed, AC_Character*, Target, UStatusBase*, Status, int, ID, int, Stacks);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusRemoved, AC_Character*, Target, int, ID);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMPYREAN_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStatusApplied OnStatusApplied;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastOnStatusApplied(AC_Character* Target, UStatusBase* Status, int ID, UTexture2D* Icon, float Duration);
	void MulticastOnStatusApplied_Implementation(AC_Character* Target, UStatusBase* Status, int ID, UTexture2D* Icon, float Duration) { OnStatusApplied.Broadcast(Target, Status, ID, Icon, Duration); }
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStatusRefreshed OnStatusRefreshed;
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastOnStatusRefreshed(AC_Character* Target, UStatusBase* Status, int ID, int Stacks);
	void MulticastOnStatusRefreshed_Implementation(AC_Character* Target, UStatusBase* Status, int ID, int Stacks) { OnStatusRefreshed.Broadcast(Target, Status, ID, Stacks); }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStatusRemoved OnStatusRemoved;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastOnStatusRemoved(AC_Character* Target, int ID);
	void MulticastOnStatusRemoved_Implementation(AC_Character* Target, int ID) { OnStatusRemoved.Broadcast(Target, ID); }
	
	UPROPERTY()
		AC_Character* Pawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Status")
		TArray<UStatusBase*> Buffs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Status")
		TArray<UStatusBase*> Debuffs;

	UFUNCTION(BlueprintCallable)
		UStatusBase* AddStatus(UStatusBase* StatusToApply, AC_Character* Caster, UAbilityBase* Ability, bool& Refreshed, bool OverrideDuration = false, float Duration = 0.f);

	UFUNCTION()
		UStatusBase* LookForStatus(UStatusBase* StatusToLookFor, AC_Character* Caster, UAbilityBase* Ability, bool& Refreshed);

	UFUNCTION()
		void RemoveStatus(UStatusBase* StatusToRemove);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
