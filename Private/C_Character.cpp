// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Character.h"

// Sets default values
AC_Character::AC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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




