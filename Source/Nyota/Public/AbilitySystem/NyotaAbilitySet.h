// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "NyotaAbilitySet.generated.h"

class UGameplayEffect;
class UGA_BaseSkill;
class UAttributeSet;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class UCustomAbilitySystemComponent;

/**
 * FNyotaAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FNyotaAbilitySet_GameplayAbility
{
    GENERATED_BODY()

    // Gameplay ability to grant.
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGA_BaseSkill> Ability;

    // Level of ability to grant.
    UPROPERTY(EditDefaultsOnly)
    int32 AbilityLevel = 1;

    // Tag used to process input for the ability.
    UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
    FGameplayTag InputTag;
};

/**
 * FNyotaAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FNyotaAbilitySet_GameplayEffect
{
    GENERATED_BODY()

    // Gameplay effect to grant.
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayEffect> GameplayEffect;

    // Level of gameplay effect to grant.
    UPROPERTY(EditDefaultsOnly)
    float EffectLevel = 1.0f;
};

/**
 * FNyotaAbilitySet_AttributeSet
 *
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FNyotaAbilitySet_AttributeSet
{
    GENERATED_BODY()

    // Gameplay effect to grant.
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UAttributeSet> AttributeSet;
};

/**
 * FNyotaAbilitySet_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FNyotaAbilitySet_GrantedHandles
{
    GENERATED_BODY()

    void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle &Handle);
    void AddGameplayEffectHandle(const FActiveGameplayEffectHandle &Handle);
    void AddAttributeSet(UAttributeSet *Set);

    void TakeFromAbilitySystem(UCustomAbilitySystemComponent *NyotaASC);

protected:
    // Handles to the granted abilities.
    UPROPERTY()
    TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

    // Handles to the granted gameplay effects.
    UPROPERTY()
    TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

    // Pointers to the granted attribute sets
    UPROPERTY()
    TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/**
 *
 */
UCLASS(BlueprintType, Const)
class NYOTA_API UNyotaAbilitySet : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UNyotaAbilitySet(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    // Grants the ability set to the specified ability system component.
    // The returned handles can be used later to take away anything that was granted.
    void GiveToAbilitySystem(
        UCustomAbilitySystemComponent *NyotaASC, FNyotaAbilitySet_GrantedHandles *OutGrantedHandles,
        UObject *SourceObject = nullptr
    ) const;

protected:
    // Gameplay abilities to grant when this ability set is granted.
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
    TArray<FNyotaAbilitySet_GameplayAbility> GrantedGameplayAbilities;

    // Gameplay effects to grant when this ability set is granted.
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
    TArray<FNyotaAbilitySet_GameplayEffect> GrantedGameplayEffects;

    // Attribute sets to grant when this ability set is granted.
    UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty = AttributeSet))
    TArray<FNyotaAbilitySet_AttributeSet> GrantedAttributes;
};
