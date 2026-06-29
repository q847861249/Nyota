// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Enums/PlayerType.h"
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

    virtual void TickComponent(
        float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction
    ) override;

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

    EPlayerType GetPlayerType() const;

protected:
    virtual void OnRegister() override;

    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    void OnPawnControllerChanged(APawn *Pawn, AController *OldController, AController *NewController);

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

    bool bHasRegisteredInitState = false;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float RotationInterpSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_Forward = 90.f; // 纯前后

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_Side = 0.f; // 纯左右

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_ForwardLeft = 45.f; // W+A

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_ForwardRight = -45.f; // W+D

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_BackLeft = -45.f; // S+A

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_BackRight = 45.f; // S+D

    float TargetYaw; // 记录目标朝向

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | PlayerType")
    EPlayerType PlayerType;
};
