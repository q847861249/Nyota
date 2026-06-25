// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "NyotaHeroComponent.generated.h"

struct FInputActionValue;
struct FInputMappingContextAndPriority;
struct FGameplayTag;

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class NYOTA_API UNyotaHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
    GENERATED_BODY()

public:
    UNyotaHeroComponent(const FObjectInitializer &ObjectInitializer);

    bool IsReadyToBindInputs() const;

    static const FName NAME_BindInputsNow;

    static const FName NAME_ActorFeatureName;

    //~ Begin IGameFrameworkInitStateInterface interface
    virtual FName GetFeatureName() const override
    {
        return NAME_ActorFeatureName;
    }

    virtual bool CanChangeInitState(
        UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
    ) const override;

    virtual void HandleChangeInitState(
        UGameFrameworkComponentManager *Manager, FGameplayTag CurrentState, FGameplayTag DesiredState
    ) override;

    virtual void OnActorInitStateChanged(const FActorInitStateChangedParams &Params) override;

    virtual void CheckDefaultInitialization() override;
    //~ End IGameFrameworkInitStateInterface interface

protected:
    virtual void OnRegister() override;

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void InitializePlayerInput(UInputComponent *PlayerInputComponent);

    void Input_AbilityInputTagPressed(const FGameplayTag &InputTag);

    void Input_AbilityInputTagReleased(const FGameplayTag &InputTag);

    void Input_Move(const FInputActionValue &InputActionValue);

    void Input_LookMouse(const FInputActionValue &InputActionValue);

    void Input_LookStick(const FInputActionValue &InputActionValue);

    UPROPERTY(EditAnywhere)
    TArray<FInputMappingContextAndPriority> DefaultInputMappings;

    // 玩家控制的 Pawn 已经完成了基础输入绑定，处于可以接收"附加输入配置"的状态。它只对真实玩家控制的角色为 true，
    // AI 控制的角色永远不会走到这一步。
    bool bReadyToBindInputs;
};
