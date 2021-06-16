// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Castbar.generated.h"

class AC_Character;
class UActiveAbilityBase;
/**
 * 
 */
UCLASS()
class EMPYREAN_API UCastbar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	UWidgetAnimation* CastAnimation;
	UPROPERTY(BlueprintReadWrite)
	UWidgetAnimation* SuccessAnimation;
	UPROPERTY(BlueprintReadWrite)
	UWidgetAnimation* FailAnimation;
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* Bar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CastTime;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CurrentTime;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* AbilityName;

	UPROPERTY(BlueprintReadOnly)
	float CastDuration;
	
	UPROPERTY(BlueprintReadOnly)
	bool Channeled;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayCastAnimation(float FinalCastTime, bool IsChanneled, float StartAtTime, bool PlayFinishedAnimation);
	void PlayCastAnimation_Implementation(float FinalCastTime, bool IsChanneled, float StartAtTime, bool PlayFinishedAnimation);

	UFUNCTION(BlueprintCallable)
	void StartCastbar(UActiveAbilityBase* Ability, float InCastTime, float StartAtTime, bool PlayFinishedAnimation);

	UFUNCTION(BlueprintCallable)
	void InitializeCastbar(AC_Character* Character);
	
	UFUNCTION(BlueprintCallable)
	void OnCastStart(UActiveAbilityBase* Ability, float InCastTime);

	UFUNCTION(BlueprintCallable)
	void OnCastEnd();
};
