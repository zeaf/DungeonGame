// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoftTargetingComponent.generated.h"


class UCameraComponent;
class AC_Character;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELENAPLAYGROUND_API USoftTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoftTargetingComponent();

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
		AC_Character* Target;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
		TSubclassOf<AActor> OutlineActor;
	
	UPROPERTY()
		AActor* CurrentOutline;

	UPROPERTY()
		AC_Character* PreviousTarget;


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

	AC_Character* GetFriendlyTarget(TArray<AActor*> Result);
	AC_Character* GetEnemyTarget(TArray<AActor*> Result);

	void RemoveActorsNotInLOS(TArray<AActor*>& Result);
};


