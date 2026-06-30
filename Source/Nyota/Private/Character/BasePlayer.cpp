

// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayer.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/BaseAttributeSet.h"
#include "Character/BasePlayerState.h"
#include "GameplayTags/GameTags.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/NyotaPawnExtensionComponent.h"
#include "AbilitySystem/NyotaAbilitySystemComponent.h"

ABasePlayer::ABasePlayer(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PawnExtensionComponent = CreateDefaultSubobject<UNyotaPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
    PawnExtensionComponent->OnAbilitySystemInitialized_RegisterAndCall(
        FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized)
    );
    PawnExtensionComponent->OnAbilitySystemUninitialized_Register(
        FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized)
    );
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent *ABasePlayer::GetAbilitySystemComponent() const
{
    ABasePlayerState *BasePlayerState = Cast<ABasePlayerState>(GetPlayerState());
    if (!IsValid(BasePlayerState))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s Is Not Valid."), *GetNameSafe(this));
        return nullptr;
    }

    return BasePlayerState->GetAbilitySystemComponent();
}

void ABasePlayer::PossessedBy(AController *NewController)
{
    Super::PossessedBy(NewController);

    if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority())
    {
        return;
    }

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);

    OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

    InitializeAttributes();

    UBaseAttributeSet *BaseAttributeSet = Cast<UBaseAttributeSet>(GetAttributeSet());
    if (!IsValid(BaseAttributeSet))
    {
        return;
    }

    GetAbilitySystemComponent()
        ->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute())
        .AddUObject(this, &ThisClass::OnHealthChange);
}

void ABasePlayer::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    if (!IsValid(GetAbilitySystemComponent()))
    {
        return;
    }

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);

    OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

    UBaseAttributeSet *BaseAttributeSet = Cast<UBaseAttributeSet>(GetAttributeSet());
    if (!IsValid(BaseAttributeSet))
    {
        return;
    }

    GetAbilitySystemComponent()
        ->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute())
        .AddUObject(this, &ThisClass::OnHealthChange);
}

UAttributeSet *ABasePlayer::GetAttributeSet() const
{
    ABasePlayerState *MyPlayerState = Cast<ABasePlayerState>(GetPlayerState());
    if (!IsValid(MyPlayerState))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: MyPlayerState Is Null."), *GetNameSafe(this));
        return nullptr;
    }

    return MyPlayerState->GetAttributeSet();
}

void ABasePlayer::BeginPlay()
{
    Super::BeginPlay();
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayer::OnOverlapBegin);
    }
}

void ABasePlayer::SetGrabbedEnemy(ABaseCharacter *Character)
{
    GrabbedEnemy = Character;
}

ABaseCharacter *ABasePlayer::GetGrabbedEnemy() const
{
    return GrabbedEnemy.Get();
}

void ABasePlayer::ResetGrabbedEnemy()
{
    GrabbedEnemy.Reset();
}

UNyotaAbilitySystemComponent *ABasePlayer::GetNyotaAbilitySystemComponent() const
{
    return Cast<UNyotaAbilitySystemComponent>(GetAbilitySystemComponent());
}

void ABasePlayer::OnOverlapBegin(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult &SweepResult
)
{
    // Check if the actor has specific tag
    if (OtherActor && OtherActor->ActorHasTag(FName("Coin")))
    {
        FGameplayEventData Payload;
        Payload.Instigator = this;
        Payload.Target = OtherActor;
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Nyota::Event_Item_PickUp, Payload);
    }
}

void ABasePlayer::OnAbilitySystemInitialized()
{
    UNyotaAbilitySystemComponent *NyotaASC = GetNyotaAbilitySystemComponent();
    check(NyotaASC);
}

void ABasePlayer::OnAbilitySystemUninitialized()
{
}
