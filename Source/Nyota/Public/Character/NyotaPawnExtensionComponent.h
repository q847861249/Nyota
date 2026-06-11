// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"

#include "NyotaPawnExtensionComponent.generated.h"

class UNyotaPawnData;
class UCustomAbilitySystemComponent;

/**
 * 核心思想：把"Actor 初始化"拆成多个独立注册的 Feature，每个 Feature 管理自己的状态转移条件。
 * PawnExtensionComponent 充当"同步屏障"——它在 DataAvailable → DataInitialized 时要求所有其他 Feature 都到达
 * DataAvailable，从而确保所有数据到位后才统一初始化。
 */
UCLASS(MinimalAPI)
class UNyotaPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    NYOTA_API UNyotaPawnExtensionComponent(const FObjectInitializer &ObjectInitializer);

    UFUNCTION(BlueprintPure, Category = "Nyota | Pawn")
    static UNyotaPawnExtensionComponent *FindPawnExtensionComponent(const AActor *Actor);

    NYOTA_API void InitializeAbilitySystem(UCustomAbilitySystemComponent *InASC, AActor *InOwnerActor);

    NYOTA_API void UninitializeAbilitySystem();

    static NYOTA_API const FName NAME_ActorFeatureName;

    // ~ Begin IGameFrameworkInitStateInterface interface
    virtual FName GetFeatureName() const override;

    NYOTA_API virtual bool CanChangeInitState(
        UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
    ) const override;

    NYOTA_API virtual void HandleChangeInitState(
        UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
    ) override;

    NYOTA_API virtual void OnActorInitStateChanged(const FActorInitStateChangedParams &Params) override;

    NYOTA_API virtual void CheckDefaultInitialization() override;
    // ~ End IGameFrameworkInitStateInterface interface

    NYOTA_API void SetPawnData(const UNyotaPawnData *InPawnData);

protected:
    NYOTA_API virtual void OnRegister() override;

    NYOTA_API virtual void BeginPlay() override;

    NYOTA_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    void OnRep_PawnData();

    // 当我们的 Pawn 成为 Ability System 的 Avatar Actor 时触发的代理
    FSimpleMulticastDelegate OnAbilitySystemInitialized;

    // 当我们的 Pawn 作为 Ability System 的 Avatar Actor 被移除时触发的代理
    FSimpleMulticastDelegate OnAbilitySystemUninitialized;

    // 用于创建角色的 Pawn 数据。可通过生成函数或放置的实例指定。
    UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "Nyota | Pawn")
    TObjectPtr<const UNyotaPawnData> PawnData;

    UPROPERTY(Transient)
    TObjectPtr<UCustomAbilitySystemComponent> AbilitySystemComponent;
};
