// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusBase.h"
#include "C_Character.h"
#include "AbilityBase.h"
#include "StatusInterface.h"

// Sets default values for this component's properties
UStatusBase::UStatusBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatusBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatusBase::RemoveStack_Implementation()
{
	if (--CurrentStacks < 1)
		Expired();
}

void UStatusBase::AddStack_Implementation()
{
}

void UStatusBase::RefreshedStatus_Implementation()
{
	if (MaxStacks > 1 && CurrentStacks < MaxStacks)
	{
		++CurrentStacks;
		AddStack();
	}

	if (CanBeRefreshed)
		GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &UStatusBase::Expired, Duration, false, 0.f);
}

void UStatusBase::Initialize_Implementation(AC_Character* Target, AC_Character* Caster, UAbilityBase* ParentAbility)
{
	this->TargetActor = Target;
	this->Instigator = Caster;
	this->Ability = ParentAbility;

	GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &UStatusBase::Expired, Duration, false, 0.f);

	//IStatusInterface* StatusInterface = Cast<IStatusInterface>(TargetActor);

	//if (StatusInterface)
	//	StatusInterface->Execute_AddStatus(TargetActor,this);

	OnApplied.Broadcast(TargetActor, this);
}

void UStatusBase::Expired_Implementation()
{
	for (auto& Effect : Effects)
		Effect->OnRemoved();

	IStatusInterface* StatusInterface = Cast<IStatusInterface>(TargetActor);

	if (StatusInterface)
		StatusInterface->Execute_IRemoveStatus(TargetActor,this);

	OnRemoved.Broadcast(TargetActor, this);
}

