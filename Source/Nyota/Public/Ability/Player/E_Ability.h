// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "E_Ability.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UE_Ability : public UBaseAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage;
};
