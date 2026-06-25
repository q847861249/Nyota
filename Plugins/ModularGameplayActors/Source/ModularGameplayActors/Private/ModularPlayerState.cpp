// Fill out your copyright notice in the Description page of Project Settings.

#include "ModularPlayerState.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModularPlayerState)

void AModularPlayerState::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerState::BeginPlay()
{
    UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
        this, UGameFrameworkComponentManager::NAME_GameActorReady
    );

    Super::BeginPlay();
}

void AModularPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

    Super::EndPlay(EndPlayReason);
}

void AModularPlayerState::Reset()
{
    Super::Reset();

    TArray<UPlayerStateComponent *> ModularComponents;
    GetComponents(ModularComponents);
    for (UPlayerStateComponent *Component : ModularComponents)
    {
        Component->Reset();
    }
}

void AModularPlayerState::CopyProperties(APlayerState *PlayerState)
{
    Super::CopyProperties(PlayerState);

    // 收集当前（旧）PlayerState 上挂载的所有 UPlayerStateComponent
    // TInlineComponentArray 和 TArray 的区别：前者在栈上预分配了少量空间（比如 8 个），
    // 避免小数组还要堆分配。PlayerState 上的组件一般没几个，用 inline 更高效。
    TInlineComponentArray<UPlayerStateComponent *> PlayerStateComponents;
    GetComponents(PlayerStateComponents);

    // 遍历每个源组件，找到目标上对应的那个组件，然后拷数据
    for (UPlayerStateComponent *SourcePSComp : PlayerStateComponents)
    {
        if (UPlayerStateComponent *TargetComp = Cast<UPlayerStateComponent>(static_cast<UObject *>(
                FindObjectWithOuter(PlayerState, SourcePSComp->GetClass(), SourcePSComp->GetFName())
            )))
        {
            SourcePSComp->CopyProperties(TargetComp);
        }
    }
}