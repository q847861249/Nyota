// Fill out your copyright notice in the Description page of Project Settings.
// NyotaAbilitySystemComponent 实现
// 基于 GameplayTag 的技能输入系统，支持按下、释放、按住三种输入模式，
// 并通过激活策略（ActivationPolicy）决定技能如何响应输入。

#include "AbilitySystem/NyotaAbilitySystemComponent.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GameplayTags/GameTags.h"

UNyotaAbilitySystemComponent::UNyotaAbilitySystemComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 初始化输入缓存，保证数组为空
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
    InputHeldSpecHandles.Reset();
}

void UNyotaAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec &AbilitySpec)
{
    Super::OnGiveAbility(AbilitySpec);

    // 检查是否需要立即激活（如被动技能）
    HandleAutoActivateAbility(AbilitySpec);
}

void UNyotaAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    // 防止遍历期间能力列表被修改，避免迭代器失效
    FScopedAbilityListLock ScopedAbilityListLock(*this);
    for (const FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        HandleAutoActivateAbility(AbilitySpec);
    }
}

void UNyotaAbilitySystemComponent::InitAbilityActorInfo(AActor *InOwnerActor, AActor *InAvatarActor)
{
    FGameplayAbilityActorInfo *ActorInfo = AbilityActorInfo.Get();

    check(ActorInfo);
    check(InOwnerActor);

    const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && InAvatarActor != ActorInfo->AvatarActor;

    Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

    if (bHasNewPawnAvatar)
    {
        // 新的 Pawn Avatar 被设置，通知所有已注册的技能
        // 同时确保技能都使用实例化策略（NonInstanced 已废弃）
        for (const FGameplayAbilitySpec &AbilitySpec : ActivatableAbilities.Items)
        {
            PRAGMA_DISABLE_DEPRECATION_WARNINGS
            ensureMsgf(
                AbilitySpec.Ability &&
                    AbilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced,
                TEXT(
                    "InitAbilityActorInfo: All Abilities should be Instanced (NonInstanced is being deprecated due to "
                    "usability issues)."
                )
            );
            PRAGMA_ENABLE_DEPRECATION_WARNINGS
        }
    }
}

void UNyotaAbilitySystemComponent::SetAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level)
{
    // 仅服务端有权修改技能等级
    if (IsValid(GetAvatarActor()) && !GetAvatarActor()->HasAuthority())
    {
        return;
    }

    if (FGameplayAbilitySpec *AbilitySpec = FindAbilitySpecFromClass(GameplayAbility))
    {
        AbilitySpec->Level = Level;

        // 标记为脏，触发网络同步
        MarkAbilitySpecDirty(*AbilitySpec);
    }
}

void UNyotaAbilitySystemComponent::AddToAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level)
{
    // 仅服务端有权修改技能等级
    if (IsValid(GetAvatarActor()) && !GetAvatarActor()->HasAuthority())
    {
        return;
    }

    if (FGameplayAbilitySpec *AbilitySpec = FindAbilitySpecFromClass(GameplayAbility))
    {
        AbilitySpec->Level += Level;

        // 标记为脏，触发网络同步
        MarkAbilitySpecDirty(*AbilitySpec);
    }
}

UGameplayAbility *UNyotaAbilitySystemComponent::GetActivatableAbilitySpecByTag(const FGameplayTag &AbilityTag)
{
    TArray<FGameplayAbilitySpec> MatchingGameplayAbilities = GetActivatableAbilities();

    if (MatchingGameplayAbilities.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No activatable abilities."));

        return nullptr;
    }

    // 遍历所有可激活技能，精确匹配 AbilityTags 中的 Tag
    for (const FGameplayAbilitySpec &AbilitySpec : MatchingGameplayAbilities)
    {
        for (const FGameplayTag &Tag : AbilitySpec.Ability->AbilityTags)
        {
            if (!Tag.MatchesTagExact(AbilityTag))
            {
                continue;
            }

            return AbilitySpec.Ability.Get();
        }
    }

    return nullptr;
}

void UNyotaAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag &InputTag)
{
    if (!InputTag.IsValid())
    {
        return;
    }

    // 匹配所有 DynamicAbilityTags 中包含该输入 Tag 的技能，将其加入按下和按住缓存
    for (const FGameplayAbilitySpec &AbilitySpec : ActivatableAbilities.Items)
    {
        if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
            InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
        }
    }
}

void UNyotaAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag &InputTag)
{
    if (!InputTag.IsValid())
    {
        return;
    }

    // 匹配所有 DynamicAbilityTags 中包含该输入 Tag 的技能，标记为释放并从按住缓存中移除
    for (const FGameplayAbilitySpec &AbilitySpec : ActivatableAbilities.Items)
    {
        if (AbilitySpec.Ability && AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
        {
            InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
            InputHeldSpecHandles.Remove(AbilitySpec.Handle);
        }
    }
}

void UNyotaAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
    // 检查是否需要拦截所有技能输入（如过场、死亡、对话等状态）
    if (HasMatchingGameplayTag(Nyota::Ability_InputBlocked))
    {
        ClearAbilityInput();

        return;
    }

    // 本帧待激活的技能列表（先收集再批量激活，避免遍历期间修改技能列表）
    static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
    AbilitiesToActivate.Reset();

    /*
     * 第一轮：处理按住输入（Hold）
     * 对于策略为 WhileInputActive 且尚未激活的技能，加入待激活队列
     * 注意：已激活的 Hold 技能不需要此阶段处理（它们在激活时已经收到了输入）
     */
    for (const FGameplayAbilitySpecHandle &SpecHandle : InputHeldSpecHandles)
    {
        const FGameplayAbilitySpec *AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
        if (!AbilitySpec)
        {
            continue;
        }

        if (!AbilitySpec->Ability)
        {
            continue;
        }

        if (!AbilitySpec->IsActive())
        {
            const UNyotaGameplayAbility *NyotaAbilityCDO = Cast<UNyotaGameplayAbility>(AbilitySpec->Ability);
            if (NyotaAbilityCDO &&
                NyotaAbilityCDO->GetActivatePolicy() == ENyotaAbilityActivationPolicy::WhileInputActive)
            {
                AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
            }
        }
    }

    /*
     * 第二轮：处理按下输入（Press）
     * - 技能已激活 → 转发 InputPressed 事件给技能实例（用于连招、蓄力等场景）
     * - 技能未激活 + OnInputTriggered 策略 → 加入待激活队列
     */
    for (const FGameplayAbilitySpecHandle &SpecHandle : InputPressedSpecHandles)
    {
        FGameplayAbilitySpec *AbilitySpec = FindAbilitySpecFromHandle(SpecHandle);
        if (!AbilitySpec)
        {
            continue;
        }

        if (!AbilitySpec->Ability)
        {
            continue;
        }

        AbilitySpec->InputPressed = true;

        if (AbilitySpec->IsActive())
        {
            // 技能正在运行，将输入事件转发进去
            AbilitySpecInputPressed(*AbilitySpec);
        }
        else
        {
            const UNyotaGameplayAbility *NyotaAbilityCDO = Cast<UNyotaGameplayAbility>(AbilitySpec->Ability);
            if (NyotaAbilityCDO &&
                NyotaAbilityCDO->GetActivatePolicy() == ENyotaAbilityActivationPolicy::OnInputTriggered)
            {
                AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
            }
        }
    }

    /*
     * 统一激活前两轮收集的所有技能。
     * 收集与激活分离，避免在遍历 ActivatableAbilities 时产生副作用。
     */
    for (const FGameplayAbilitySpecHandle &AbilitySpecHandle : AbilitiesToActivate)
    {
        TryActivateAbility(AbilitySpecHandle);
    }

    /*
     * 第三轮：处理释放输入（Release）
     * 仅对已激活的技能转发释放事件（未激活的技能不需要释放通知）
     */
    for (const FGameplayAbilitySpecHandle &AbilitySpecHandle : InputReleasedSpecHandles)
    {
        FGameplayAbilitySpec *AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
        if (!AbilitySpec)
        {
            continue;
        }

        if (!AbilitySpec->Ability)
        {
            continue;
        }

        AbilitySpec->InputPressed = false;

        if (AbilitySpec->IsActive())
        {
            AbilitySpecInputReleased(*AbilitySpec);
        }
    }

    /*
     * 清理本帧的按下和释放缓存。
     * 按住缓存（InputHeldSpecHandles）不清理 —— 它是跨帧持久状态，
     * 仅在 AbilityInputTagReleased 时移除对应条目。
     */
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
}

void UNyotaAbilitySystemComponent::ClearAbilityInput()
{
    // 清空所有输入缓存（通常在输入被阻塞时调用）
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
    InputHeldSpecHandles.Reset();
}

void UNyotaAbilitySystemComponent::SetTagRelationshipMapping(UNyotaAbilityTagRelationshipMapping *NewMapping)
{
    TagRelationshipMapping = NewMapping;
}

void UNyotaAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec &Spec)
{
    Super::AbilitySpecInputPressed(Spec);

    // 仅当技能已激活时才转发输入事件 —— 通过 PredictionKey 确保网络预测正确同步
    if (Spec.IsActive())
    {
        PRAGMA_DISABLE_DEPRECATION_WARNINGS

        const UGameplayAbility *Instance = Spec.GetPrimaryInstance();
        const FPredictionKey &PredictionKey = Instance
                                                  ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey()
                                                  : Spec.ActivationInfo.GetActivationPredictionKey();

        PRAGMA_ENABLE_DEPRECATION_WARNINGS

        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, PredictionKey);
    }
}

void UNyotaAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec &Spec)
{
    Super::AbilitySpecInputReleased(Spec);

    // 仅当技能已激活时才转发释放事件 —— 通过 PredictionKey 确保网络预测正确同步
    if (Spec.IsActive())
    {
        PRAGMA_DISABLE_DEPRECATION_WARNINGS

        const UGameplayAbility *Instance = Spec.GetPrimaryInstance();
        const FPredictionKey &PredictionKey = Instance
                                                  ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey()
                                                  : Spec.ActivationInfo.GetActivationPredictionKey();

        PRAGMA_ENABLE_DEPRECATION_WARNINGS

        InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, PredictionKey);
    }
}

void UNyotaAbilitySystemComponent::HandleAutoActivateAbility(const FGameplayAbilitySpec &AbilitySpec)
{
    if (!IsValid(AbilitySpec.Ability))
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Tags: %s"), *AbilitySpec.Ability->AbilityTags.ToString());

    // 检查技能的 AbilityTags 中是否包含 Ability_ActivateOnGive，
    // 如果有则立即激活（用于被动技能、"获得即生效"的技能）
    for (const FGameplayTag &Tag : AbilitySpec.Ability->AbilityTags)
    {
        if (!Tag.MatchesTagExact(Nyota::Ability_ActivateOnGive))
        {
            continue;
        }

        TryActivateAbility(AbilitySpec.Handle);
    }
}
