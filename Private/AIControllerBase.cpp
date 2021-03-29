// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "C_character.h"
#include "ThreatComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void AAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (UpdateThreatTarget)
	{
		AC_Character* Temp = GetThreatTarget();
		if (!Temp) return;
		if (CurrentTarget != Temp)
		{
			CurrentTarget = Temp;
			K2_SetFocus(CurrentTarget);
			OnTargetChanged();
		}
	}
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControlledPawn = Cast<AC_Character>(InPawn);
	if (ControlledPawn)
		ThreatComponent = Cast<UThreatComponent>(ControlledPawn->GetComponentByClass(UThreatComponent::StaticClass()));
}

AC_Character* AAIControllerBase::GetCurrentTarget() const
{
	return CurrentTarget;
}

AC_Character* AAIControllerBase::GetThreatTarget() const
{
	return ThreatComponent ? ThreatComponent->GetHighestThreatActor() : CurrentTarget;
}

void AAIControllerBase::SetCurrentTarget(AC_Character* Target)
{
	CurrentTarget = Target;
	if (Target)
	{
		//SetActorTickEnabled(false);
		K2_SetFocus(Target);
	}
	else SetActorTickEnabled(true);
}


