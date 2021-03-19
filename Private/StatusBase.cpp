// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusBase.h"
#include "C_Character.h"
#include "AbilityBase.h"
#include "EffectBase.h"
#include "StatusInterface.h"

// Sets default values for this component's properties
UStatusBase::UStatusBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
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
		Expired(true);
}

void UStatusBase::MulticastAfterInitialize_Implementation(AC_Character* Target, AC_Character* Caster, UAbilityBase* ParentAbility)
{
	this->TargetActor = Target;
	this->Instigator = Caster;
	this->Ability = ParentAbility;

	InitializeEffects();
}

void UStatusBase::AddStack_Implementation()
{
	if (CurrentStacks < MaxStacks) ++CurrentStacks;
}

UEffectBase* UStatusBase::DuplicateEffect(UEffectBase* In)
{
	return DuplicateObject<UEffectBase>(In, this);
}

void UStatusBase::RefreshedStatus_Implementation()
{
	if (MaxStacks > 1 && CurrentStacks < MaxStacks)
	{
		++CurrentStacks;
		AddStack();
	}

	if (CanBeRefreshed)
	{
		FTimerDelegate ExpiredDelegate;
		ExpiredDelegate.BindUFunction(this, FName("Expired"), false);

		GetWorld()->GetTimerManager().SetTimer(DurationTimer, ExpiredDelegate, Duration, false);
	}
}

void UStatusBase::Initialize_Implementation(AC_Character* Target, AC_Character* Caster, UAbilityBase* ParentAbility)
{
	this->TargetActor = Target;
	this->Instigator = Caster;
	this->Ability = ParentAbility;

	FTimerDelegate ExpiredDelegate;
	ExpiredDelegate.BindUFunction(this, FName("Expired"), false);
	
	GetWorld()->GetTimerManager().SetTimer(DurationTimer, ExpiredDelegate, Duration, false);

	for (UEffectBase* Effect : Effects)
	{
		if (!Effect) continue;
		if (Effect->SelfOnly && TargetActor != Instigator)
			break;
		Effect->Initialize(this);
	}

	MulticastAfterInitialize(Target, Caster, Ability);
	
	OnApplied.Broadcast(TargetActor, this);
}

void UStatusBase::Expired_Implementation(bool WasRemoved)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	
	if (WasRemoved)
		for (auto& Effect : Effects)
			Effect->OnRemoved();
	else
		for (auto& Effect : Effects)
			Effect->OnExpired();

	IStatusInterface* StatusInterface = Cast<IStatusInterface>(TargetActor);

	MulticastRemoved();
	
	OnRemoved.Broadcast(TargetActor, this);

	if (StatusInterface)
		StatusInterface->Execute_IRemoveStatus(TargetActor,this);

	
}

