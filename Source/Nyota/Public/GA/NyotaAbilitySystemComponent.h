// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NyotaAbilitySystemComponent.generated.h"

class UNyotaAbilityTagRelationshipMapping;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYOTA_API UNyotaAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UNyotaAbilitySystemComponent(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void OnGiveAbility(FGameplayAbilitySpec &AbilitySpec) override;

    virtual void OnRep_ActivateAbilities() override;

    virtual void InitAbilityActorInfo(AActor *InOwnerActor, AActor *InAvatarActor) override;

    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    void SetAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level);

    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    void AddToAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level = 1);

    UFUNCTION(BlueprintCallable, Category = "Nyota | Ability")
    UGameplayAbility *GetActivatableAbilitySpecByTag(const FGameplayTag &AbilityTag);

    void AbilityInputTagPressed(const FGameplayTag &InputTag);

    void AbilityInputTagReleased(const FGameplayTag &InputTag);

    void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

    void ClearAbilityInput();

    void SetTagRelationshipMapping(UNyotaAbilityTagRelationshipMapping *NewMapping);

protected:
    virtual void AbilitySpecInputPressed(FGameplayAbilitySpec &Spec) override;

    virtual void AbilitySpecInputReleased(FGameplayAbilitySpec &Spec) override;

    UPROPERTY()
    TObjectPtr<UNyotaAbilityTagRelationshipMapping> TagRelationshipMapping;

    TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

    TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

    TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

private:
    void HandleAutoActivateAbility(const FGameplayAbilitySpec &AbilitySpec);
};
