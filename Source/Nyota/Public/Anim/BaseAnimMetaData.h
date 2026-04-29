// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "Animation/AnimMetaData.h"
#include "BaseAnimMetaData.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UBaseAnimMetaData : public UAnimMetaData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    FGameplayTag MontageTag;
};
