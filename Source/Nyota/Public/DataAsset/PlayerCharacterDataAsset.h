// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerCharacterDataAsset.generated.h"


class UTexture2D;
class UGameplayAbility;
USTRUCT(BlueprintType)
struct FSkillUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText SlotName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UTexture2D> AbilityIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AbilityName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine="true"))
    FText AbilityDesc;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UGameplayAbility> AbilityClass;
};
UCLASS()
class NYOTA_API UPlayerCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UTexture2D> Avatar;

	UPROPERTY(EditDefaultsOnly)
    TArray<FSkillUIData> Skills;
    // UPROPERTY(EditDefaultsOnly, Category="Skills")
    // FSkillUIData Passive;

    // UPROPERTY(EditDefaultsOnly, Category="Skills")
    // FSkillUIData Q;

    // UPROPERTY(EditDefaultsOnly, Category="Skills")
    // FSkillUIData E;

    // UPROPERTY(EditDefaultsOnly, Category="Skills")
    // FSkillUIData R;


};
