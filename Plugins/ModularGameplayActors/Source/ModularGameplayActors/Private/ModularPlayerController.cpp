// Fill out your copyright notice in the Description page of Project Settings.

#include "ModularPlayerController.h"

#include "Components/ControllerComponent.h"
#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModularPlayerController)

void AModularPlayerController::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

    Super::EndPlay(EndPlayReason);
}

void AModularPlayerController::ReceivedPlayer()
{
    // ──── 第一步：通知组件管理器 "Actor 就绪了" ────
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
        this, UGameFrameworkComponentManager::NAME_GameActorReady
    );

    // ──── 第二步：父类逻辑 ────
    Super::ReceivedPlayer();

    // ──── 第三步：把这个事件转发给所有 UControllerComponent ────
    TArray<UControllerComponent *> ModularComponents;
    GetComponents(ModularComponents);
    for (UControllerComponent *Component : ModularComponents)
    {
        Component->ReceivedPlayer();
    }
}

void AModularPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    TArray<UControllerComponent *> ModularComponents;
    GetComponents(ModularComponents);
    for (UControllerComponent *Component : ModularComponents)
    {
        Component->PlayerTick(DeltaTime);
    }
}