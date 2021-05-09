// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Misc/DateTime.h"
#include "AnimBPBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class EMPYREAN_API UAnimBPBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	APawn* Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	bool isAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	bool IsAccelerating = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	float YawDelta;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	float Roll;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	float Yaw;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	float TurnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	bool FullBody;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	UPawnMovementComponent* MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	bool IsHit;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	TArray<UAnimSequence*> HitReactions;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	int RandHitReactIndex;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Animation")
	FDateTime LastHitReact;
	
private:
	UFUNCTION(BlueprintCallable)
	void InitializeAnimation();
	UFUNCTION(BlueprintCallable)
	void UpdateAnimation(float DeltaTime, FVector LastUpdateVelocity);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayHitReactAnim();
	virtual void PlayHitReactAnim_Implementation();
};
