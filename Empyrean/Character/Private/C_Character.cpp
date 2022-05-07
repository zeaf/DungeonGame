// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Character.h"

#include "AbilityCastingComponent.h"
#include "DungeonCharacterPlayerController.h"
#include "HealthComponent.h"
#include "StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AC_Character::AC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Health = CreateDefaultSubobject<UHealthComponent>("Health");

	StatusComponent = CreateDefaultSubobject<UStatusComponent>("Status");
	
	AbilityCastingComponent = CreateDefaultSubobject<UAbilityCastingComponent>("Ability Component");

	DamageResistance = CombatAttribute::CreateCombatAttributeMap<EGameDamageType>();
	DamageFactors = CombatAttribute::CreateCombatAttributeMap<EGameDamageType>();
	CombatAttributes = CombatAttribute::CreateCombatAttributeMap<ECombatAttributeName>();
}

void AC_Character::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	for (auto& It : DamageResistance)
		It.Value.GetFinalValue();
	for (auto& It : DamageFactors)
		It.Value.GetFinalValue();
	for (auto& It : CombatAttributes)
		It.Value.GetFinalValue();

}

void AC_Character::MulticastUpdateMovementSpeed_Implementation(const float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

// Called when the game starts or when spawned
void AC_Character::BeginPlay()
{
	Super::BeginPlay();
	Dead = false;
	DungeonPlayerController = Cast<ADungeonCharacterPlayerController>(GetController());

	if (HasAuthority())
		CombatAttributes[ECombatAttributeName::MovementSpeed].OnValueChange.AddUniqueDynamic(this, &AC_Character::MulticastUpdateMovementSpeed);
}

// Called every frame
void AC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AC_Character::CheckHostility(AActor* ActorToCheck)
{
	if (const auto ActorAsChar = Cast<AC_Character>(ActorToCheck))
		return this->Team != ActorAsChar->Team;
	return false;
}

float AC_Character::GetCombatAttributeValue(ECombatAttributeName Attribute)
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

FHealingOutcome AC_Character::OnHealingReceived_Implementation(FCharacterDamageEvent Event)
{
	return Health->OnHealReceived(Event);
}

FDamageOutcome AC_Character::OnDamageReceived_Implementation(FCharacterDamageEvent Event)
{
	return Health->OnHit(Event);
}

