// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "PlayerListSection.generated.h"

/**
 * 
 */
class UImage;
class UProgressBar;
class UAbilitySystemComponent;
class APlayerCharacter;
UCLASS()
class NYOTA_API UPlayerListSection : public UCommonUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Avatar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ScoreBar;

	TObjectPtr<APlayerCharacter> PC;
	TObjectPtr<UAbilitySystemComponent> ASC;

	UFUNCTION()
	void Inital();
	void UnBind();


	void OnHealthValueChange(const struct FOnAttributeChangeData& Data);

	void OnScoreValueChange(const struct FOnAttributeChangeData& Data);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override; 

public:
	void SetASC(UAbilitySystemComponent* RepASC);
};
