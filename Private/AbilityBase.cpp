// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"

#include "C_Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/LineBatchComponent.h"
#include "DrawDebugHelpers.h"
#include "StatusComponent.h"

// Sets default values for this component's properties
UAbilityBase::UAbilityBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Caster = Cast<AC_Character>(GetOwner());
	// ...
}


// Called when the game starts
void UAbilityBase::BeginPlay()
{
	Super::BeginPlay();

	this->SetComponentTickEnabled(false);
	// ...
	
}


// Called every frame
void UAbilityBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<AC_Character*> UAbilityBase::GetTargetsInRadius(const FVector Center, const float Radius, const bool IgnoreSelf, const bool Friendly,
	const bool Enemy, const bool DrawDebug)
{
	if (DrawDebug)
		DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Emerald, false, 3.f, 0, 2);

	TArray<AActor*> IgnoredActors;
	if (IgnoreSelf) IgnoredActors.Add(Caster);

	TArray<AActor*> OverlapResult;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Center, Radius, ObjectsToTrace,
	                                          AC_Character::StaticClass(), IgnoredActors, OverlapResult);

	TArray<AC_Character*> Targets;

	for (AActor* Target : OverlapResult)
	{
		AC_Character* AsChar = Cast<AC_Character>(Target);
		bool IsEnemy = Caster->CheckHostility(AsChar);

		if (IsEnemy && Enemy) 
			Targets.Add(AsChar);
		else if (!IsEnemy && Friendly) 
			Targets.Add(AsChar);
	}

	return Targets;
}

void UAbilityBase::DealDamage(AC_Character* Target, FCharacterDamageEvent Event, 
	float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool& IsKillingBlow)
{
	if (!Target) return;
	//float DamageDealt, DamageAbsorbed;
	//bool IsCrit, IsKillingBlow;
	Target->OnDamageReceived(Event, DamageDealt, DamageAbsorbed, IsCrit, IsKillingBlow);
	OnDealtDamage.Broadcast(Target, DamageDealt+DamageAbsorbed, this);
}

UStatusBase* UAbilityBase::ApplyStatus(AC_Character* Target, int StatusIndex, bool& Refreshed)
{
	if (!Target) return nullptr;
	if (!StatusToApply[StatusIndex]) return nullptr;
	return Target->Execute_AddStatus(Target, StatusToApply[StatusIndex], Caster, this, Refreshed);
}

void UAbilityBase::LookForStatus(AC_Character* Target, bool IsDebuff, TSubclassOf<UStatusBase> StatusToLookFor, 
	UStatusBase*& FoundStatus, int& Stacks,	bool OwnOnly)
{
	if (!StatusToLookFor) return;
	if (!Target) return;

	TArray<UStatusBase*>& StatusArray = IsDebuff ? Target->StatusComponent->Debuffs : Target->StatusComponent->Buffs;

	for (UStatusBase* Status : StatusArray)
	{
		if (Status->GetClass() == StatusToLookFor && (Status->Instigator == Caster || !OwnOnly))
		{
			FoundStatus = Status;
			Stacks = FoundStatus->CurrentStacks;
		}
	}
}

void UAbilityBase::HealUnit(AC_Character* Target, FCharacterDamageEvent Event)
{
	float Healing;
	bool IsCrit;
	Target->OnHealingReceived(Event, Healing, IsCrit);
	OnHealedUnit.Broadcast(Target, Healing, this);
}

void UAbilityBase::ConeTrace(FVector ConeOrigin, bool TargetFriendly, bool TargetEnemy, bool IgnoreSelf, float Range, float ConeAngle,
                             bool DrawDebug, TArray<AC_Character*>& CharactersHit)
{
	AC_Character* Owner = Cast<AC_Character>(GetOwner());
	
	if (DrawDebug)
	{
		float s = sin(FMath::DegreesToRadians(ConeAngle));
		float c = cos(FMath::DegreesToRadians(ConeAngle));

		FVector fwv = Owner->GetActorForwardVector();
		
		float x = fwv.X * c - fwv.Y * s;
		float y = fwv.X * s + fwv.Y * c;

		GetWorld()->PersistentLineBatcher->DrawLine(ConeOrigin, 
			ConeOrigin+FVector(x, y, fwv.Z)*Range,
			FColor::Red, 1, 5, 3);
		
		s = sin(FMath::DegreesToRadians(-ConeAngle));
		c = cos(FMath::DegreesToRadians(-ConeAngle));

		x = fwv.X * c - fwv.Y * s;
		y = fwv.X * s + fwv.Y * c;
		
		GetWorld()->PersistentLineBatcher->DrawLine(ConeOrigin, 
			ConeOrigin+FVector(x, y, fwv.Z)*Range,
			FColor::Red, 1, 5, 3);
	}
	
	TArray<FHitResult> Overlaps;

	auto OverlapParams = FCollisionQueryParams(FName("cone"), true, IgnoreSelf ? GetOwner() : nullptr);
	
	GetWorld()->SweepMultiByChannel(Overlaps, ConeOrigin, ConeOrigin, Owner->GetActorRotation().Quaternion(), 
		ECC_GameTraceChannel12,	FCollisionShape::MakeSphere(Range), OverlapParams, FCollisionResponseParams());

	TMap<AC_Character*, bool> ActorsHit;
	TMap<AActor*, bool> IgnoredActors;

	for (auto &Hit : Overlaps)
	{
		if (DrawDebug)
			GetWorld()->PersistentLineBatcher->DrawLine(Hit.ImpactPoint, 
			Hit.ImpactPoint, FColor::Red, 1, 5, 1);
		
		if (IgnoredActors.Find(Hit.GetActor()))
			continue;

		AC_Character* ActorAsChar = Cast<AC_Character>(Hit.GetActor());
		if (!ActorAsChar)
			IgnoredActors.Add(Hit.GetActor(),1);
		else
		{
			if (!ActorsHit.Find(ActorAsChar))
			{
				bool Affiliation = Owner->CheckHostility(ActorAsChar);

				if ((TargetFriendly && !Affiliation) || (TargetEnemy && Affiliation))
				{
					float DotProduct = (Hit.ImpactPoint - ConeOrigin).CosineAngle2D(Owner->GetActorForwardVector());
					if (DotProduct > FMath::Cos(FMath::DegreesToRadians(ConeAngle)))
					{
						CharactersHit.Add(Cast<AC_Character>(ActorAsChar));
						ActorsHit.Add(ActorAsChar,1);
					}
				}
			}
		}
	}
}





