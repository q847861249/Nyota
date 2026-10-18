// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NyotaStatusComponent.h"

#include "AbilitySystemComponent.h"
#include "Character/BaseCharacter.h"
#include "GameplayEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/GameTags.h"
#include "Runtime/AIModule/Classes/AIController.h"

bool UNyotaStatusComponent::ApplyStatusEffect(AActor *Target, TSubclassOf<UGameplayEffect> StatusEffect, float Duration)
{
    ABaseCharacter *Character = Cast<ABaseCharacter>(Target);

    if (!IsValid(Character) || !IsValid(StatusEffect) || !Character->IsAlive())
    {
        return false;
    }

    UAbilitySystemComponent *ASC = Character->GetAbilitySystemComponent();

    if (!IsValid(ASC))
    {
        return false;
    }

    // 控制免疫统一在这里拦截（如冲锋中的野猪授予 Status_Immune_Control），调用方无需各自检查
    if (ASC->HasMatchingGameplayTag(Nyota::Status_Immune_Control))
    {
        return false;
    }

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    ContextHandle.AddSourceObject(Character);

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(StatusEffect, 1.f, ContextHandle);

    if (!SpecHandle.IsValid())
    {
        return false;
    }

    // Duration>0 时覆盖 GE 资产里的时长，方便技能按参数表配置不同眩晕时间
    if (Duration > 0.f)
    {
        SpecHandle.Data->SetDuration(Duration, true);
    }

    return ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()).WasSuccessfullyApplied();
}

void UNyotaStatusComponent::BeginPlay()
{
    Super::BeginPlay();

    ABaseCharacter *Character = Cast<ABaseCharacter>(GetOwner());

    if (!IsValid(Character))
    {
        return;
    }

    if (UAbilitySystemComponent *ASC = Character->GetAbilitySystemComponent(); IsValid(ASC))
    {
        CachedAbilitySystemComponent = ASC;

        RegisterBlockedTagEvents();
    }
    else
    {
        // 玩家角色的 ASC 挂在 PlayerState 上，组件 BeginPlay 时可能尚未就绪，等广播后补注册
        Character->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
    }
}

void UNyotaStatusComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (IsValid(CachedAbilitySystemComponent))
    {
        if (MovementTagEventHandle.IsValid())
        {
            CachedAbilitySystemComponent->UnregisterGameplayTagEvent(
                MovementTagEventHandle, Nyota::Status_Blocked_Movement, EGameplayTagEventType::AnyCountChange
            );
        }

        if (AITagEventHandle.IsValid())
        {
            CachedAbilitySystemComponent->UnregisterGameplayTagEvent(
                AITagEventHandle, Nyota::Status_Blocked_AI, EGameplayTagEventType::AnyCountChange
            );
        }

        if (SlowedTagEventHandle.IsValid())
        {
            CachedAbilitySystemComponent->UnregisterGameplayTagEvent(
                SlowedTagEventHandle, Nyota::Status_Slowed, EGameplayTagEventType::AnyCountChange
            );
        }
    }

    Super::EndPlay(EndPlayReason);
}

void UNyotaStatusComponent::OnASCInitialized(UAbilitySystemComponent *ASC, UAttributeSet *AS)
{
    if (!IsValid(ASC) || IsValid(CachedAbilitySystemComponent))
    {
        return;
    }

    CachedAbilitySystemComponent = ASC;

    RegisterBlockedTagEvents();
}

void UNyotaStatusComponent::RegisterBlockedTagEvents()
{
    // 用 Tag 计数事件而非布尔翻转：多个状态同时冻结同一维度时，最后一个解除才恢复
    MovementTagEventHandle = CachedAbilitySystemComponent
                                 ->RegisterGameplayTagEvent(Nyota::Status_Blocked_Movement, EGameplayTagEventType::AnyCountChange)
                                 .AddUObject(this, &ThisClass::OnBlockedMovementChanged);

    AITagEventHandle = CachedAbilitySystemComponent
                           ->RegisterGameplayTagEvent(Nyota::Status_Blocked_AI, EGameplayTagEventType::AnyCountChange)
                           .AddUObject(this, &ThisClass::OnBlockedAIChanged);

    SlowedTagEventHandle = CachedAbilitySystemComponent
                               ->RegisterGameplayTagEvent(Nyota::Status_Slowed, EGameplayTagEventType::AnyCountChange)
                               .AddUObject(this, &ThisClass::OnSlowedChanged);
}

void UNyotaStatusComponent::OnBlockedMovementChanged(const FGameplayTag Tag, int32 NewCount)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    UCharacterMovementComponent *Movement = IsValid(Character) ? Character->GetCharacterMovement() : nullptr;

    if (!IsValid(Movement))
    {
        return;
    }

    if (NewCount > 0 && !bMovementFrozen)
    {
        SavedMovementMode = Movement->MovementMode;
        Movement->StopMovementImmediately();
        Movement->SetMovementMode(MOVE_None);

        bMovementFrozen = true;
    }
    else if (NewCount == 0 && bMovementFrozen)
    {
        // 恢复冻结前的模式，避免把 Falling 错误恢复成 Walking 导致角色贴地
        Movement->SetMovementMode(SavedMovementMode);

        bMovementFrozen = false;
    }
}

void UNyotaStatusComponent::OnSlowedChanged(const FGameplayTag Tag, int32 NewCount)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    UCharacterMovementComponent *Movement = IsValid(Character) ? Character->GetCharacterMovement() : nullptr;

    if (!IsValid(Movement))
    {
        return;
    }

    if (NewCount > 0 && !bSlowed)
    {
        // 记录并压低地面速度；多层减速靠 Tag 计数保证只在第一层生效，最后一层解除才恢复
        SavedMaxWalkSpeed = Movement->MaxWalkSpeed;
        Movement->MaxWalkSpeed = SavedMaxWalkSpeed * SlowSpeedMultiplier;

        bSlowed = true;
    }
    else if (NewCount == 0 && bSlowed)
    {
        Movement->MaxWalkSpeed = SavedMaxWalkSpeed;

        bSlowed = false;
    }
}

void UNyotaStatusComponent::OnBlockedAIChanged(const FGameplayTag Tag, int32 NewCount)
{
    APawn *Pawn = IsValid(GetOwner()) ? Cast<APawn>(GetOwner()) : nullptr;
    AAIController *AIC = IsValid(Pawn) ? Cast<AAIController>(Pawn->GetController()) : nullptr;

    if (!IsValid(AIC))
    {
        return;
    }

    if (NewCount > 0 && !bAIFrozen)
    {
        AIC->StopMovement();
        AIC->SetActorTickEnabled(false);

        bAIFrozen = true;
    }
    else if (NewCount == 0 && bAIFrozen)
    {
        AIC->SetActorTickEnabled(true);

        bAIFrozen = false;
    }
}
