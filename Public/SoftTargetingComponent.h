// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoftTargetingComponent.generated.h"


class UCameraComponent;
class AC_Character;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFriendlyTargetUpdate, AC_Character*, NewTarget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyTargetUpdate, AC_Character*, NewTarget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendlyTargetCleared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyTargetCleared);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELENAPLAYGROUND_API USoftTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USoftTargetingComponent();

	UFUNCTION(BlueprintCallable)
		void InitializeTargeting();

	UPROPERTY(BlueprintAssignable)
		FOnFriendlyTargetCleared OnFriendlyTargetCleared;

	UPROPERTY(BlueprintAssignable)
		FOnEnemyTargetCleared OnEnemyTargetCleared;

	UPROPERTY(BlueprintAssignable)
		FOnEnemyTargetUpdate OnEnemyTargetUpdate;

	UPROPERTY(BlueprintAssignable)
		FOnFriendlyTargetUpdate OnFriendlyTargetUpdate;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
		TArray<TEnumAsByte<EObjectTypeQuery>> TracedTypes;

	UPROPERTY()
		TArray<AActor*> IgnoredActors;

	UPROPERTY()
		TArray<AActor*> OverlapResult;

	UPROPERTY()
		AC_Character* Pawn;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		AC_Character* FriendlyTarget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		AC_Character* EnemyTarget;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		TSubclassOf<AActor> OutlineActorFriendly;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		TSubclassOf<AActor> OutlineActorEnemy;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		float MaxDotProduct = 0.99;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		float MinDotProduct = 0.965;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		float MaxTargetingRange = 2850;
	
	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		UCameraComponent* PawnCamera;

	UPROPERTY()
		AActor* CurrentOutlineFriendly;

	UPROPERTY()
		AC_Character* PreviousTargetFriendly;

	UPROPERTY()
		AActor* CurrentOutlineEnemy;

	UPROPERTY()
		AC_Character* PreviousTargetEnemy;

	FCollisionObjectQueryParams Params;
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TargetEnemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TargetFriendlies = true;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetTargets(AC_Character*& Friendly, AC_Character*& Enemy);
	void RemoveActorsNotInLOS(TArray<AActor*>& Result);

	AC_Character* CalculateScores(TArray<AC_Character*> Targets);

	void MoveOutline(AC_Character* Target, bool Enemy);

};