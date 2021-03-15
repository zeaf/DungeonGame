// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectPeriodic.h"

#include "StatusBase.h"

void UEffectPeriodic::Initialize(UStatusBase* ParentStatus)
{
	Super::Initialize(ParentStatus);
	Duration = ParentStatus->Duration;

	if (NumberOfTicks > 0)
		TickInterval = Duration / NumberOfTicks;
	else if (!(TickInterval > 0 && TickInterval < Duration))
		TickInterval = 1.f;

	NumberOfTicks = Duration / TickInterval;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UEffectPeriodic::OnTick, TickInterval, true, TickInterval);
	
}

void UEffectPeriodic::OnRemoved()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UEffectPeriodic::OnExpired()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	
	for (int i = TicksDone; i < NumberOfTicks; ++i)
		OnTick();
}

void UEffectPeriodic::OnTick_Implementation()
{
	if (++TicksDone == NumberOfTicks)
	{
		TimerHandle.Invalidate();
	}
}
