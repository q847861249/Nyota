// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "Q_Ability.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UQ_Ability : public UBaseAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage;
};
