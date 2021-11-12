// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Character.h"

#include "AbilityCastingComponent.h"
#include "DungeonCharacterPlayerController.h"
#include "HealthComponent.h"
#include "StatusComponent.h"
#include "SoftTargetingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AC_Character::AC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Health = CreateDefaultSubobject<UHealthComponent>("Health");

	StatusComponent = CreateDefaultSubobject<UStatusComponent>("Status");
	
	AbilityCastingComponent = CreateDefaultSubobject<UAbilityCastingComponent>("Ability Component");

	//SoftTargeting = CreateDefaultSubobject<USoftTargetingComponent>("SoftTargeting");
	
}

// Called when the game starts or when spawned
void AC_Character::BeginPlay()
{
	Super::BeginPlay();
	Dead = false;
	DungeonPlayerController = Cast<ADungeonCharacterPlayerController>(GetController());
	
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

float AC_Character::GetCombatAttributeValue(CombatAttributeName Attribute)
{
	return CombatAttributes[Attribute].CurrentValue;
}

UStatusBase* AC_Character::AddStatus_Implementation(UStatusBase* Status, AC_Character* Caster, UAbilityBase* Ability, bool& Refreshed, bool OverrideDuration, float Duration)
{
	return StatusComponent->AddStatus(Status, Caster, Ability, Refreshed, OverrideDuration, Duration);
}

void AC_Character::IRemoveStatus_Implementation(UStatusBase* Status)
{
	StatusComponent->RemoveStatus(Status);
}

void AC_Character::OnDeath_Implementation()
{
	Dead = true;

	if (HasAuthority())
	{
		for (UStatusBase* Aura : StatusComponent->Buffs)
			Cast<IStatusInterface>(this)->Execute_IRemoveStatus(this,Aura);
		for (UStatusBase* Aura : StatusComponent->Debuffs)
			Cast<IStatusInterface>(this)->Execute_IRemoveStatus(this,Aura);
	}

	GetCharacterMovement()->DisableMovement();
	OnCharacterDeath.Broadcast(this);
}

void AC_Character::OnHealingReceived_Implementation(FCharacterDamageEvent Event, float& Healing, float& Overhealing, bool& IsCrit)
{
	Health->OnHealReceived(Event, Healing, Overhealing, IsCrit);
}

void AC_Character::OnDamageReceived_Implementation(FCharacterDamageEvent Event, float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool& IsKillingBlow, float&
                                                   UnmitigatedDamage, float& DamageWithNoModifiers, float& DamageWithoutIncreases)
{
	Health->OnHit(Event, DamageDealt, DamageAbsorbed, IsCrit, IsKillingBlow, UnmitigatedDamage, DamageWithNoModifiers, DamageWithoutIncreases);
}

