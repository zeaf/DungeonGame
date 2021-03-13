// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/LineBatchComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UAbilityBase::UAbilityBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

