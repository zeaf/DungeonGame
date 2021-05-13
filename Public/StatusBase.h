// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "C_Character.h"
//#include "EffectBase.h"
#include "Components/ActorComponent.h"
#include "StatusBase.generated.h"

class AC_Character;
class UAbilityBase;
class UEffectBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAppliedDelegate, AC_Character*, Actor, UStatusBase*, Status);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRefreshedDelegate, AC_Character*, Actor, UStatusBase*, Status);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRemovedDelegate, AC_Character*, Actor, UStatusBase*, Status);

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMPYREAN_API UStatusBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusBase();

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadwrite, Category = "Status")
		TArray<UEffectBase*> Effects;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay, Category = "Status")
		AC_Character* TargetActor;
	UPROPERTY(BlueprintReadOnly, AdvancedDisplay, Category = "Status")
		AC_Character* Instigator;

	UPROPERTY(BlueprintReadOnly, AdvancedDisplay, Category = "Status")
		UAbilityBase* Ability;

	UPROPERTY(BlueprintReadOnly, Replicated, AdvancedDisplay, Category = "Status")
		int CurrentStacks = 1;

	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere, Category = "Status")
		float Duration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		bool CanBeRefreshed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		bool IsDebuff;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		int MaxStacks;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status")
		FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Status", meta = (Multiline = "true"))
		FText Description;
	UPROPERTY(BlueprintReadWrite, Replicated, EditAnywhere, Category = "Status")
		UTexture2D* Icon;

	FTimerHandle DurationTimer;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Status")
		FAppliedDelegate OnApplied;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Status")
		FRefreshedDelegate OnRefreshed;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Status")
		FRemovedDelegate OnRemoved;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Initialize(AC_Character* Target, AC_Character* Caster, UAbilityBase* ParentAbility);
	virtual void Initialize_Implementation(AC_Character* Target, AC_Character* Caster, UAbilityBase* ParentAbility);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Expired(bool WasRemoved);
	virtual void Expired_Implementation(bool WasRemoved);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void RefreshedStatus();
	virtual void RefreshedStatus_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void AddStack();
	virtual void AddStack_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void RemoveStack();
	virtual void RemoveStack_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAfterInitialize(AC_Character* Target, AC_Character* Caster, UAbilityBase* ParentAbility);
	virtual void MulticastAfterInitialize_Implementation(AC_Character* Target, AC_Character* Caster, UAbilityBase* ParentAbility);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void InitializeEffects();
	virtual void InitializeEffects_Implementation() {};


	UFUNCTION(NetMulticast, Reliable)
		void MulticastExpired();
	virtual void MulticastExpired_Implementation() { RemoveEffects(); };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RemoveEffects();
	virtual void RemoveEffects_Implementation() {};

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRemoved();
	virtual void MulticastRemoved_Implementation(){ RemoveEffects(); };
	
	UFUNCTION(BlueprintCallable)
	UEffectBase* DuplicateEffect(UEffectBase* In);
};
