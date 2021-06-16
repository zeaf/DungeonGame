// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Castbar.h"

#include "AbilityCastingComponent.h"
#include "C_Character.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

void UCastbar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentTime->SetText(UKismetTextLibrary::Conv_FloatToText(Bar->Percent * CastDuration, HalfFromZero, 
		false, true, 1, 10, 1, 1));
}

void UCastbar::PlayCastAnimation_Implementation(float FinalCastTime, bool IsChanneled, float StartAtTime,
	bool PlayFinishedAnimation)
{
}

void UCastbar::StartCastbar(UActiveAbilityBase* Ability, float InCastTime, float StartAtTime,
                            bool PlayFinishedAnimation)
{
	if (IsAnimationPlaying(SuccessAnimation) || IsAnimationPlaying(FailAnimation))
		StopAllAnimations();

	AbilityName->SetText(UKismetTextLibrary::Conv_NameToText(Ability->AbilityName));

	SetRenderOpacity(1.f);

	SetVisibility(ESlateVisibility::HitTestInvisible);

	Icon->GetDynamicMaterial()->SetTextureParameterValue("Icon", Ability->Icon);

	CastTime->SetText(UKismetTextLibrary::Conv_FloatToText(InCastTime, HalfFromZero, false, false, 1, 5, 1, 1));
	CastDuration = InCastTime;

	PlayCastAnimation(InCastTime, Ability->IsChanneled, StartAtTime, PlayFinishedAnimation);
	Channeled = Ability->IsChanneled;
}

void UCastbar::InitializeCastbar(AC_Character* Character)
{
	Character->AbilityCastingComponent->OnCastStart.AddDynamic(this, &UCastbar::OnCastStart);
	Character->AbilityCastingComponent->OnCastEnd.AddDynamic(this, &UCastbar::OnCastEnd);
}

void UCastbar::OnCastStart(UActiveAbilityBase* Ability, float InCastTime)
{
	StartCastbar(Ability, InCastTime, 0, true);
}

void UCastbar::OnCastEnd()
{
	StopAnimation(CastAnimation);
}
