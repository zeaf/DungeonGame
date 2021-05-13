// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusBase.h"
#include "C_Character.h"
#include "AbilityBase.h"
#include "EffectBase.h"
#include "StatusInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStatusBase::UStatusBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	auto Outer = GetOuter();
	UAbilityBase* Spell = Cast<UAbilityBase>(Outer);
	if (Spell)
	{
		if (!Icon)
			Icon = Spell->Icon;

		if (Name == NAME_None)
			Name = Spell->AbilityName;
	}
	
}


void UStatusBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStatusBase, Duration);
	DOREPLIFETIME(UStatusBase, Icon);
}

// Called when the game starts
void UStatusBase::BeginPlay()
{
	Super::BeginPlay();
	//if (!Icon) Icon = Ability->
	// ...
	auto Outer = GetOuter();
	UAbilityBase* Spell = Cast<UAbilityBase>(Outer);
	if (Spell)
	{
		if (!Icon)
			Icon = Spell->Icon;

		if (Name == NAME_None)
			Name = Spell->AbilityName;
	}
	
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

	if (!Icon) Icon = Ability->Icon;
	if (Name == NAME_None) Name = Ability->AbilityName;
	
	InitializeEffects();
}

void UStatusBase::AddStack_Implementation()
{
	if (MaxStacks > 1 && CurrentStacks < MaxStacks) ++CurrentStacks;
}

UEffectBase* UStatusBase::DuplicateEffect(UEffectBase* In)
{
	return DuplicateObject<UEffectBase>(In, this);
}

void UStatusBase::RefreshedStatus_Implementation()
{
	AddStack();

	if (CanBeRefreshed)
	{
		for (UEffectBase* Effect : Effects)
			Effect->OnRefreshed();
		
		FTimerDelegate ExpiredDelegate;
		ExpiredDelegate.BindUFunction(this, FName("Expired"), false);

		GetWorld()->GetTimerManager().SetTimer(DurationTimer, ExpiredDelegate, Duration, false);
		OnRefreshed.Broadcast(TargetActor, this);
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

