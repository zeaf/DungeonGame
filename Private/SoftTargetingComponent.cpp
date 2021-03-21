// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftTargetingComponent.h"
#include "C_Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


DECLARE_STATS_GROUP(TEXT("DungeonGame"), STATGROUP_DungeonGame, STATCAT_Advanced);

DECLARE_CYCLE_STAT(TEXT("SoftTargeting - Overlap"), STAT_SoftTargetOverlap, STATGROUP_DungeonGame);

DECLARE_CYCLE_STAT(TEXT("SoftTargeting - Remove out of LOS"), STAT_SoftTargetLOS, STATGROUP_DungeonGame);

DECLARE_CYCLE_STAT(TEXT("SoftTargeting - GetBestTarget"), STAT_SoftTargetBestTarget, STATGROUP_DungeonGame);


// Sets default values for this component's properties
USoftTargetingComponent::USoftTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);
	// ...
}


// Called when the game starts
void USoftTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	AController* Controller = Cast<AController>(GetOuter());
	
	Pawn = Controller->GetPawn<AC_Character>();
	TracedTypes.Add(ObjectTypeQuery3);
	Params.AddObjectTypesToQuery(ECC_WorldDynamic);
	Params.AddObjectTypesToQuery(ECC_WorldStatic);
	
	IgnoredActors.Add(Pawn);
	if (!Pawn) return;
	CurrentOutlineFriendly = GetWorld()->SpawnActor(OutlineActorFriendly);
	CurrentOutlineEnemy = GetWorld()->SpawnActor(OutlineActorEnemy);
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	SetComponentTickEnabled(true);
}


// Called every frame
void USoftTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//FVector TraceStart = Pawn->GetActorLocation() + FVector(0,0,300);
	//FVector TraceEnd = TraceStart + Pawn->GetControlRotation().Vector() * 2500.f;
	//FRotator TraceRot = UKismetMathLibrary::FindLookAtRotation(TraceStart, TraceEnd);
	//TArray<FHitResult> OutHits;

	
	//UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), TraceStart, TraceEnd, FVector(100, 1200, 500), TraceRot,
	//	TracedTypes, true, IgnoredActors, EDrawDebugTrace::None, OutHits, true);

	//for (FHitResult Hit : OutHits)
	//{
	//	AActor* HitActor = Hit.GetActor();
	//	if (HitActor)
	//		if (HitActor->IsA(AC_Character::StaticClass()))
	//			Result.AddUnique(Hit.GetActor());
	//}
	
	{
		SCOPE_CYCLE_COUNTER(STAT_SoftTargetOverlap);
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Pawn->GetActorLocation(), MaxTargetingRange, TracedTypes,
			AC_Character::StaticClass(), IgnoredActors, OverlapResult);
	}

	{
		SCOPE_CYCLE_COUNTER(STAT_SoftTargetLOS);
		RemoveActorsNotInLOS(OverlapResult);		
	}

	{
		SCOPE_CYCLE_COUNTER(STAT_SoftTargetBestTarget);
		GetTargets(OverlapResult, FriendlyTarget, EnemyTarget);
	}

	MoveOutline(FriendlyTarget, false);
	MoveOutline(EnemyTarget, true);
	

}

void USoftTargetingComponent::MoveOutline(AC_Character* Target, bool Enemy)
{
	AActor*& CurrentOutline = Enemy ? CurrentOutlineEnemy : CurrentOutlineFriendly;
	AC_Character*& PreviousTarget = Enemy ? PreviousTargetEnemy : PreviousTargetFriendly;
	
	if (!Target)
	{
		CurrentOutline->SetActorHiddenInGame(true);
		if (PreviousTarget)
			PreviousTarget->GetMesh()->SetRenderCustomDepth(false);
		PreviousTarget = nullptr;
	}
	else if (PreviousTarget != Target)
	{
		Target->GetMesh()->SetRenderCustomDepth(true);
		Target->GetMesh()->SetCustomDepthStencilValue(Enemy ? 1 : 2);
		if (PreviousTarget)
			PreviousTarget->GetMesh()->SetRenderCustomDepth(false);
		PreviousTarget = Target;

		CurrentOutline->AttachToActor(Target, Rules);
		CurrentOutline->SetActorHiddenInGame(false);
		FVector origin, Extents;
		Target->GetActorBounds(true, origin, Extents);

		CurrentOutline->SetActorScale3D(Extents / 80);
	}
}


void USoftTargetingComponent::GetTargets(TArray<AActor*> Result, AC_Character*& Friendly, AC_Character*& Enemy)
{
	TArray<AC_Character*> Friendlies;
	TArray<AC_Character*> Enemies;
	
	for (AActor* Char : Result)
	{
		AC_Character* AsChar = Cast<AC_Character>(Char);
		if (AsChar)
		{
			bool IsFriendly = !Pawn->CheckHostility(AsChar);
			if (IsFriendly && TargetFriendlies)
				Friendlies.Add(AsChar);
			else if (!IsFriendly && TargetEnemies)
				Enemies.Add(AsChar);
		}
	}

	Friendly = TargetFriendlies ? CalculateScores(Friendlies) : nullptr;
	Enemy = TargetEnemies ? CalculateScores(Enemies) : nullptr;
	

}

AC_Character* USoftTargetingComponent::CalculateScores(TArray<AC_Character*> Targets)
{
	TMap<AC_Character*, float> DotProducts;
	TMap<AC_Character*, float> Distances;

	AC_Character* BestTarget = nullptr;

	float DistanceMax = 0;
	float DistanceMin = 100000;
	float RotMax = 0;

	TArray<AC_Character*> EligibleTargets;
	
	for (AC_Character* AsChar : Targets)
	{
		FVector LineToTarget = AsChar->GetActorLocation() - Pawn->GetActorLocation();
		LineToTarget.Normalize();
		float dot = FVector::DotProduct(LineToTarget, PawnCamera->GetForwardVector());
		float Distance = FVector::Distance(AsChar->GetActorLocation(), Pawn->GetActorLocation());
		FVector2D InRange = FVector2D(0, MaxTargetingRange); //TODO expose range as variable
		FVector2D OutRange = FVector2D(MinDotProduct, MaxDotProduct);
		float DotMappedToDistance = FMath::GetMappedRangeValueClamped(InRange, OutRange, Distance);
		if (dot > DotMappedToDistance)
		{
			DistanceMax = Distance > DistanceMax ? Distance : DistanceMax;
			RotMax = dot > RotMax ? dot : RotMax;
			DistanceMin = Distance < DistanceMin ? Distance : DistanceMin;
			DotProducts.Add(AsChar, dot);
			Distances.Add(AsChar, Distance);
			EligibleTargets.Add(AsChar);
		}
	}

	float BestScore = 1000000;
	for (AC_Character* Char : EligibleTargets)
	{
		FVector2D InRange = FVector2D(0, DistanceMax);
		FVector2D OutRange = FVector2D(1, 7);
		FVector2D InRangeRot = FVector2D(MinDotProduct, RotMax);
		FVector2D OutRangeRot = FVector2D(4.5, 1.f);
		float RotScore = FMath::Pow(FMath::GetMappedRangeValueClamped(InRangeRot, OutRangeRot, DotProducts[Char]), 2.8);
		float DistScore = FMath::GetMappedRangeValueClamped(InRange, OutRange, Distances[Char]);
		float CurrentScore = RotScore + DistScore;

		if (CurrentScore < BestScore)
		{
			BestScore = CurrentScore;
			BestTarget = Char;
		}
	}

	return BestTarget;
}

void USoftTargetingComponent::RemoveActorsNotInLOS(TArray<AActor*>& Result)
{
	TArray<AActor*> ToRemove;
	for (AActor* Actor : Result)
	{
		FHitResult Hit;
		bool Blocking = GetWorld()->LineTraceSingleByObjectType(Hit, Pawn->GetActorLocation(), Actor->GetActorLocation(),
			Params);
		if (Blocking) ToRemove.Add(Actor);
	}

	for (AActor* Actor : ToRemove)
		Result.Remove(Actor);
}

