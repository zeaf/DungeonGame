// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_StatusList.generated.h"

class UWidget_SpellIcon;
class UStatusBase;
class UWidget_AuraWrapper;
class AC_Character;
class UWrapBox;

UENUM(BlueprintType)
enum class AuraListIncludedAuras : uint8
{
	Both				UMETA(DisplayName = "Both"),
	OwnOnly				UMETA(DisplayName = "Own Only"),
	ExternalOnly		UMETA(DisplayName = "External Only")
};

UCLASS()
class EMPYREAN_API UWidget_StatusList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	int MaxAuras;

	UPROPERTY(BlueprintReadWrite)
	int CurrentAuras;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool BuffContainer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AuraListIncludedAuras IncludedAuras;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UWidget_AuraWrapper> AuraWrapperClass;
	
	UPROPERTY()
	TWeakObjectPtr<AC_Character> Character;

	UPROPERTY(BlueprintReadOnly)
	TMap<int, UWidget_AuraWrapper*> AuraMap;

	UFUNCTION()
	virtual bool IsAuraIncluded(UStatusBase* Status);
	UFUNCTION()
	virtual void OnStatusApplied(AC_Character* Target, UStatusBase* Status, int ID, UTexture2D* Icon, float Duration);
	UFUNCTION()
	virtual void OnStatusRemoved(AC_Character* Target, int ID);
	UFUNCTION()
	virtual void OnStatusRefreshed(AC_Character* Target, UStatusBase* Status, int ID, int Stacks);

	UFUNCTION(BlueprintCallable)
	virtual void AddNewAura(UWidget_AuraWrapper* NewAura) {}

	UFUNCTION(BlueprintCallable)
	virtual void RemoveAura(int ID) {}
public:
	UFUNCTION(BlueprintCallable)
	virtual void InitializeAuraList(AC_Character* Target);
};
