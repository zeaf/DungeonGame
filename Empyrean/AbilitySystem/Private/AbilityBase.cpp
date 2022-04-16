// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"

#include "AbilityCastingComponent.h"
#include "C_Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/LineBatchComponent.h"
#include "DrawDebugHelpers.h"
#include "StatusComponent.h"
//#include "Chaos/AABB.h"

// Sets default values for this component's properties
UAbilityBase::UAbilityBase()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	Caster = Cast<AC_Character>(GetOwner());
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

UAbilityBase* UAbilityBase::LookForAbility(const TSubclassOf<UAbilityBase> AbilityClass)
{
	for (UAbilityBase* Ability : Caster->AbilityCastingComponent->Abilities)
	{
		if (Ability->GetClass() == AbilityClass)
			return Ability;
	}
	return nullptr;
}

UAbilityBase* UAbilityBase::GetPassive()
{
	return Caster->AbilityCastingComponent->PassiveAbility;
}

TArray<AC_Character*> UAbilityBase::GetTargetsInRadius(const FVector Center, const float Radius,
                                                       int32 TargetsAllowed, const bool DrawDebug)
{
	if (DrawDebug)
		DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Emerald, false, 3.f, 0, 2);

	TArray<AActor*> IgnoredActors;
	if (!(TargetsAllowed & (uint8)ETargetingMask::TargetSelf))
	{
		IgnoredActors.Add(Caster);
	}

	TArray<AActor*> OverlapResult;

	TArray<UPrimitiveComponent*> OverlapComponents;
	const bool bOverlapped = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), Center, Radius, ObjectsToTrace, NULL, IgnoredActors, OverlapComponents);

	// For some reason components without Generate Overlap Events are still overlapped in C++, so we have to remove those
	TArray<UPrimitiveComponent*> OverlapComponentsFixed;

	for (UPrimitiveComponent* Comp : OverlapComponents)
	{
		if (Comp->GetGenerateOverlapEvents())
			OverlapComponentsFixed.Add(Comp);
	}
	
	if (bOverlapped)
	{
		UKismetSystemLibrary::GetActorListFromComponentList(OverlapComponentsFixed, AC_Character::StaticClass(), OverlapResult);
	}

	// Only consider targets that fit our targeting criteria
	TArray<AC_Character*> Targets;

	for (AActor* Target : OverlapResult)
	{
		AC_Character* AsChar = Cast<AC_Character>(Target);
		const bool IsEnemy = Caster->CheckHostility(AsChar);
		
		if (AsChar->Dead) continue;
		
		if (IsEnemy && (TargetsAllowed & (uint8)ETargetingMask::TargetEnemy))
			Targets.Add(AsChar);
		else if (!IsEnemy && (TargetsAllowed & (uint8)ETargetingMask::TargetFriendly))
			Targets.Add(AsChar);
	}

	return Targets;
}

void UAbilityBase::DealDamage_Implementation(AC_Character* Target, FCharacterDamageEvent Event, 
	float& DamageDealt, float& DamageAbsorbed, bool& IsCrit, bool& IsKillingBlow, float& UnmitigatedDamage, float& DamageWithNoModifiers, float& DamageWithoutIncreases)
{
	DamageDealt = 0;
	DamageAbsorbed = 0;
	IsCrit = false;
	IsKillingBlow = false;
	UnmitigatedDamage = false;
	DamageWithNoModifiers = 0;
	DamageWithoutIncreases = 0;
	if (!Target) return;
	if (Target->Dead) return;
	Target->OnDamageReceived(Event, DamageDealt, DamageAbsorbed, IsCrit, IsKillingBlow, UnmitigatedDamage, DamageWithNoModifiers, DamageWithoutIncreases);
	OnDealtDamage.Broadcast(Target, Event, DamageDealt + DamageAbsorbed, IsCrit, IsKillingBlow, UnmitigatedDamage, DamageWithNoModifiers, DamageWithoutIncreases);
}

UStatusBase* UAbilityBase::ApplyStatus(AC_Character* Target, int StatusIndex, bool& Refreshed, bool OverrideDuration, float Duration)
{
	if (!Target) return nullptr;
	
	if (!StatusToApply[StatusIndex]) return nullptr;
	auto Status = Target->Execute_AddStatus(Target, StatusToApply[StatusIndex], Caster, this, Refreshed, OverrideDuration, Duration);

	return Status;
}

void UAbilityBase::LookForStatus(AC_Character* Target, const bool IsDebuff, TSubclassOf<UStatusBase> StatusToLookFor, 
	UStatusBase*& FoundStatus, int& Stacks,	const bool OwnOnly)
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

void UAbilityBase::HealUnit(AC_Character* Target, FCharacterDamageEvent Event, float& Healing, float& Overhealing, bool& IsCrit)
{
	Target->OnHealingReceived(Event, Healing, Overhealing, IsCrit);
	OnHealedUnit.Broadcast(Target, Event, Healing, IsCrit);
}

AC_Character* UAbilityBase::GetMainTarget(TArray<AC_Character*> Targets)
{
	AC_Character* CurrentTarget = nullptr;
	float MinDistance = 1000000;
	for (AC_Character* Char : Targets)
	{
		const float TempDist = FVector::Dist(Caster->GetActorLocation(), Char->GetActorLocation());
	
		if (TempDist < MinDistance)
		{
			MinDistance = TempDist;
			CurrentTarget = Char;
		}
	}
	return CurrentTarget;
}

void UAbilityBase::ConeTrace(FVector ConeOrigin, FVector ForwardVector, TArray<AC_Character*>& CharactersHit, int32 TargetsAllowed,
                             const float Range, const float ConeAngle, const bool DrawDebug, ECollisionChannel
                             TraceChannel)
{
	if (ForwardVector == FVector(0, 0, 0))
		ForwardVector = Caster->GetActorForwardVector();

	if (DrawDebug)
	{
		float s = sin(FMath::DegreesToRadians(ConeAngle));
		float c = cos(FMath::DegreesToRadians(ConeAngle));

		float x = ForwardVector.X * c - ForwardVector.Y * s;
		float y = ForwardVector.X * s + ForwardVector.Y * c;

		GetWorld()->PersistentLineBatcher->DrawLine(ConeOrigin, 
			ConeOrigin+FVector(x, y, ForwardVector.Z)*Range,
			FColor::Red, 1, 5, 3);
		
		s = sin(FMath::DegreesToRadians(-ConeAngle));
		c = cos(FMath::DegreesToRadians(-ConeAngle));

		x = ForwardVector.X * c - ForwardVector.Y * s;
		y = ForwardVector.X * s + ForwardVector.Y * c;
		
		GetWorld()->PersistentLineBatcher->DrawLine(ConeOrigin, 
			ConeOrigin+FVector(x, y, ForwardVector.Z)*Range,
			FColor::Red, 1, 5, 3);
	}
	
	TArray<FHitResult> Overlaps;

	auto OverlapParams = FCollisionQueryParams(FName("cone"), 
		true, 
		!(TargetsAllowed & (uint8)ETargetingMask::TargetSelf)  ? GetOwner() : nullptr);
	
	GetWorld()->SweepMultiByChannel(Overlaps, ConeOrigin, ConeOrigin, ForwardVector.ToOrientationRotator().Quaternion(), 
	TraceChannel, FCollisionShape::MakeSphere(Range), OverlapParams, FCollisionResponseParams());

	TMap<AC_Character*, bool> ActorsHit;
	TMap<AActor*, bool> IgnoredActors;

	for (auto &Hit : Overlaps)
	{
		if (DrawDebug)
			DrawDebugPoint(GetWorld(),Hit.ImpactPoint, 5, FColor::Red, false, 5);
		
		if (IgnoredActors.Find(Hit.GetActor()))
			continue;

		AC_Character* ActorAsChar = Cast<AC_Character>(Hit.GetActor());
		if (!ActorAsChar)
			IgnoredActors.Add(Hit.GetActor(),true);
		else
		{
			if (!ActorsHit.Find(ActorAsChar))
			{
				bool Affiliation = Caster->CheckHostility(ActorAsChar);

				if ((TargetsAllowed & (uint8)ETargetingMask::TargetFriendly && !Affiliation) || 
					(TargetsAllowed & (uint8)ETargetingMask::TargetEnemy && Affiliation))
				{
					float DotProduct = (Hit.ImpactPoint - ConeOrigin).CosineAngle2D(ForwardVector);
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





