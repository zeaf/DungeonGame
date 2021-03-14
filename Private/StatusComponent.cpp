// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"
#include "C_Character.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Pawn = Cast<AC_Character>(GetOuter());
	// ...
	
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UStatusBase* UStatusComponent::AddStatus(UStatusBase* StatusToApply)
{
	if (Pawn)
		if (!Pawn->Dead)
			return LookForStatus(StatusToApply);
	return nullptr;
}

UStatusBase* UStatusComponent::LookForStatus(UStatusBase* StatusToLookFor)
{
	if (!StatusToLookFor)
		return nullptr;

	TArray<UStatusBase*> StatusArray = StatusToLookFor->IsDebuff ? Debuffs : Buffs;
	
	for (UStatusBase* Status : StatusArray)
	{
		if (Status->GetClass() == StatusToLookFor->GetClass() && Status->Instigator == StatusToLookFor->Instigator)
		{
			Status->RefreshedStatus();
			return Status;
		}
	}

	if (Pawn->HasAuthority())
	{
		UStatusBase* NewStatus = DuplicateObject<UStatusBase>(StatusToLookFor, Pawn);
		StatusArray.Add(NewStatus);
		NewStatus->Initialize(Pawn, StatusToLookFor->Instigator, StatusToLookFor->Ability);
		return NewStatus;
	}
	return nullptr;
}

void UStatusComponent::RemoveStatus(UStatusBase* StatusToRemove)
{
	if (Pawn)
		if (!Pawn->Dead)
		{
			TArray<UStatusBase*> StatusArray = StatusToRemove->IsDebuff ? Debuffs : Buffs;
			StatusArray.Remove(StatusToRemove);
		}
}

