// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimBPBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Misc/DateTime.h"
#include "Kismet/KismetSystemLibrary.h"


void UAnimBPBase::InitializeAnimation()
{
	Pawn = TryGetPawnOwner();
	if (Pawn)
		MovementComponent = Pawn->GetMovementComponent();
}

void UAnimBPBase::UpdateAnimation(float DeltaTime, FVector LastUpdateVelocity)
{
	if (Pawn)
	{
		IsInAir = MovementComponent->IsFalling();
		Speed = Pawn->GetVelocity().Size();
		FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(Pawn->GetBaseAimRotation() ,Pawn->GetActorRotation());

		Roll = DeltaRotator.Roll;
		Pitch = DeltaRotator.Pitch;
		Yaw = DeltaRotator.Yaw;
		
		FVector Velocity = MovementComponent->Velocity;
		FRotator RotFromXVel = LastUpdateVelocity.ToOrientationRotator();
		
		TurnRotation = UKismetMathLibrary::NormalizedDeltaRotator(RotFromXVel, Pawn->GetActorRotation()).Yaw;
		IsAccelerating = Velocity.Size() > 0.f;
		FullBody = GetCurveValue("FullBody") > 0.f;
	}
}

void UAnimBPBase::PlayHitReactAnim_Implementation()
{
	auto time = FDateTime::Now();

	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("HitReact"), true, true, FColor::Red, 2.f);
	
	if ((time - LastHitReact).GetTotalMilliseconds() > 200)
	{
		LastHitReact = time;
		IsHit = true;
		RandHitReactIndex = FMath::RandRange(0, HitReactions.Num()-1);
	}
}

//void UAnimBPBase::PlayHitReactAnim_Implementation()
//{

//}
