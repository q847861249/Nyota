// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Character/NyotaPawnExtensionComponent.h"
#include "Containers/Ticker.h"
#include "Input/NyotaInputComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTags/GameTags.h"
ABaseCharacter::ABaseCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent *ABaseCharacter::GetAbilitySystemComponent() const
{
    return nullptr;
}

UAttributeSet *ABaseCharacter::GetAttributeSet() const
{
    return nullptr;
}

bool ABaseCharacter::IsAlive() const
{
    return bAlive;
}

void ABaseCharacter::SetAlive(bool bAliveStatus)
{
    bAlive = bAliveStatus;
}

void ABaseCharacter::InitializeAttributes() const
{
    checkf(InitializeAttributesEffect, TEXT("InitializeAttributesEffect not set."));

    FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle =
        GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributesEffect, 1.f, ContextHandle);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ABaseCharacter::OnHealthChange(const FOnAttributeChangeData &AttributeChangeData)
{
    if (AttributeChangeData.NewValue <= 0.f)
    {
        OnDeath();
    }
}

void ABaseCharacter::OnDeath()
{
    bAlive = false;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s has die!"), *GetName()));
    }
}

UInputComponent *ABaseCharacter::CreatePlayerInputComponent()
{
    return NewObject<UNyotaInputComponent>(this, UNyotaInputComponent::StaticClass(), TEXT("NyotaInputComponent0"));
}

void ABaseCharacter::PawnClientRestart()
{
    Super::PawnClientRestart();

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("[InitTrace] PawnClientRestart on %s | InputComponent=%s"),
        *GetName(),
        InputComponent ? TEXT("valid") : TEXT("NULL")
    );

    // InputComponent 在 Super::PawnClientRestart 里通过 SetupPlayerInputComponent 创建完毕，
    // 但 InitState 链可能在之前因为缺少 InputComponent 而卡住了。
    // 这里重新触发检查，让 HeroComponent 完成 DataAvailable → DataInitialized 的推进。
    if (UNyotaPawnExtensionComponent *PawnExtComp = UNyotaPawnExtensionComponent::FindPawnExtensionComponent(this))
    {
        PawnExtComp->CheckDefaultInitialization();

        // 上面的调用可能因为 re-entrancy 导致 PawnExtComp 自己没能推进到 DataInitialized。
        // 延迟一帧再试一次——此时 HeroComponent 的 DI 状态已完全提交，条件全部满足。
        FTSTicker::GetCoreTicker().AddTicker(
            FTickerDelegate::CreateWeakLambda(
                this,
                [this](float) -> bool {
                    if (UNyotaPawnExtensionComponent *ExtComp =
                            UNyotaPawnExtensionComponent::FindPawnExtensionComponent(this))
                    {
                        ExtComp->CheckDefaultInitialization();
                    }
                    return false; // 只执行一次
                }
            ),
            0.0f
        );
    }
    else
    {
        UE_LOG(
            LogTemp, Error, TEXT("[InitTrace] PawnClientRestart: NO PawnExtensionComponent found on %s!"), *GetName()
        );
    }
}

void ABaseCharacter::OnRespawn()
{
    bAlive = true;
}

EPlayerType ABaseCharacter::GetPlayerType() const
{
    return PlayerType;
}

FVector ABaseCharacter::GetForwardDirection() const
{
    // if (GetPlayerType() == EPlayerType::Crab)
    // {
    //     return GetActorRightVector();
    // }

    return GetActorForwardVector();
}
