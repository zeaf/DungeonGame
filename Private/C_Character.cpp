// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Character.h"
#include "HealthComponent.h"
#include "StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AC_Character::AC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Health = CreateDefaultSubobject<UHealthComponent>("Health");
	Health->Pawn = this;

	Status = CreateDefaultSubobject<UStatusComponent>("Status");
	Status->Pawn = this;
	
}

// Called when the game starts or when spawned
void AC_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AC_Character::CheckHostility(AActor* ActorToCheck)
{
	auto ActorAsChar = Cast<AC_Character>(ActorToCheck);
	if (ActorAsChar)
		return this->Team != ActorAsChar->Team;
	return false;
}

void AC_Character::AddStatus_Implementation(UStatusBase* Status)
{
}

void AC_Character::IRemoveStatus_Implementation(UStatusBase* Status)
{
}

void AC_Character::OnDeath_Implementation()
{
	Dead = true;

	if (HasAuthority())
	{
		for (UStatusBase* Aura : Status->Buffs)
			Cast<IStatusInterface>(this)->IRemoveStatus(Aura);
		for (UStatusBase* Aura : Status->Debuffs)
			Cast<IStatusInterface>(this)->IRemoveStatus(Aura);
	}

	GetCharacterMovement()->DisableMovement();
	OnCharacterDeath.Broadcast(this);
}




