// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/NyotaAbilitySystemComponent.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Abilities/NyotaGameplayAbility.h"
#include "GameplayTags/GameTags.h"

UNyotaAbilitySystemComponent::UNyotaAbilitySystemComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
    InputHeldSpecHandles.Reset();
}

void UNyotaAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec &AbilitySpec)
{
    Super::OnGiveAbility(AbilitySpec);

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
        // Notify all abilities that a new pawn avatar has been set
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
    if (IsValid(GetAvatarActor()) && !GetAvatarActor()->HasAuthority())
    {
        return;
    }

    if (FGameplayAbilitySpec *AbilitySpec = FindAbilitySpecFromClass(GameplayAbility))
    {
        AbilitySpec->Level = Level;

        MarkAbilitySpecDirty(*AbilitySpec);
    }
}

void UNyotaAbilitySystemComponent::AddToAbilityLevel(TSubclassOf<UGameplayAbility> GameplayAbility, int32 Level)
{
    if (IsValid(GetAvatarActor()) && !GetAvatarActor()->HasAuthority())
    {
        return;
    }

    if (FGameplayAbilitySpec *AbilitySpec = FindAbilitySpecFromClass(GameplayAbility))
    {
        AbilitySpec->Level += Level;

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
    if (HasMatchingGameplayTag(Nyota::Ability_InputBlocked))
    {
        ClearAbilityInput();

        return;
    }

    static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
    AbilitiesToActivate.Reset();

    /*
     * 处理所有在按住输入时激活的技能。
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
     * 处理所有在本帧被按下输入的能力。
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
     * Try to activate all the abilities that are from presses and holds.
     */
    for (const FGameplayAbilitySpecHandle &AbilitySpecHandle : AbilitiesToActivate)
    {
        TryActivateAbility(AbilitySpecHandle);
    }

    /*
     * Process all abilities that had their input released this frame.
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
     * Clear the cached ability handles.
     */
    InputPressedSpecHandles.Reset();
    InputReleasedSpecHandles.Reset();
}

void UNyotaAbilitySystemComponent::ClearAbilityInput()
{
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
    for (const FGameplayTag &Tag : AbilitySpec.Ability->AbilityTags)
    {
        if (!Tag.MatchesTagExact(Nyota::Ability_ActivateOnGive))
        {
            continue;
        }

        TryActivateAbility(AbilitySpec.Handle);
    }
}