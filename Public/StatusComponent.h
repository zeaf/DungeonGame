// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityBase.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


class AC_Character;
class UStatusBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELENAPLAYGROUND_API UStatusComponent : public UActorComponent
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

	UPROPERTY()
		AC_Character* Pawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		TArray<UStatusBase*> Buffs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		TArray<UStatusBase*> Debuffs;

	UFUNCTION(BlueprintCallable)
		UStatusBase* AddStatus(UStatusBase* StatusToApply, AC_Character* Caster, UAbilityBase* Ability);

	UFUNCTION()
		UStatusBase* LookForStatus(UStatusBase* StatusToLookFor, AC_Character* Caster, UAbilityBase* Ability);

	UFUNCTION()
		void RemoveStatus(UStatusBase* StatusToRemove);
};
