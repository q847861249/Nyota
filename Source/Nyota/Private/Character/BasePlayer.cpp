

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

void ABasePlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    /*
    APlayerController *PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem *LocalPlayerSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (!LocalPlayerSubsystem)
    {
        return;
    }
    if (InputMappingContext)
    {
        LocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
        LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: InputMappingContext Is Null."), *GetNameSafe(this));
    }
    */

    /*
    if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::LookInput);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s: LookAction Is Null."), *GetNameSafe(this));
        }

        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::MoveInput);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s: MoveAction Is Null."), *GetNameSafe(this));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s: EnhancedInputComponent Is Null."), *GetNameSafe(this));
    }
    */
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

    GiveDefaultAbility();

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

void ABasePlayer::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FRotator CurrentRotation = GetActorRotation();
    FRotator NewRotation =
        FMath::RInterpTo(CurrentRotation, FRotator(0, TargetYaw, 0), DeltaSeconds, RotationInterpSpeed);
    SetActorRotation(NewRotation);
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

void ABasePlayer::MoveInput(const FInputActionValue &Value)
{
    if (!Controller)
    {
        return;
    }

    const FVector2D Axis = Value.Get<FVector2D>();
    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, ControlRotation.Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    auto SetTargetYaw = [&](float Offset) {
        // 防止 ControlRotation.Yaw - 135.f 和 + 135.f 在角度接近 ±180°
        // 边界时可能会出现角度翻转，导致螃蟹突然反向旋转一大圈。
        TargetYaw = FRotator::NormalizeAxis(ControlRotation.Yaw + Offset);
    };

    bool bHasHorizontal = !FMath::IsNearlyZero(Axis.X);
    bool bHasVertical = !FMath::IsNearlyZero(Axis.Y);

    if (GetPlayerType() == EPlayerType::Crab)
    {
        if (bHasHorizontal && bHasVertical)
        {
            if (Axis.Y > 0.f && Axis.X < 0.f) // W+A
            {
                SetTargetYaw(YawOffset_ForwardLeft);
            }
            else if (Axis.Y > 0.f && Axis.X > 0.f) // W+D
            {
                SetTargetYaw(YawOffset_ForwardRight);
            }
            else if (Axis.Y < 0.f && Axis.X < 0.f) // S+A
            {
                SetTargetYaw(YawOffset_BackLeft);
            }
            else if (Axis.Y < 0.f && Axis.X > 0.f) // S+D
            {
                SetTargetYaw(YawOffset_BackRight);
            }
        }
        else if (bHasHorizontal)
        {
            SetTargetYaw(YawOffset_Side); // 逆时针90°，屁股朝摄像机
        }
        else
        {
            SetTargetYaw(YawOffset_Forward); // 回到默认，右钳朝摄像机
        }
    }

    AddMovementInput(ForwardDirection, Axis.Y);
    AddMovementInput(RightDirection, Axis.X);
}

void ABasePlayer::LookInput(const FInputActionValue &Value)
{
    if (!Controller)
    {
        return;
    }

    const FVector2D LookVector2D = Value.Get<FVector2D>();

    AddControllerYawInput(LookVector2D.X);
    AddControllerPitchInput(LookVector2D.Y);
}

void ABasePlayer::OnLightAttack_Started()
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayTagContainer Container;
    Container.AddTag(Nyota::Ability_LightAttack);
    ASC->TryActivateAbilitiesByTag(Container);
}

void ABasePlayer::OnLeftLightAttack_Started()
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayTagContainer Container;
    Container.AddTag(Nyota::Ability_LeftLightAttack);
    ASC->TryActivateAbilitiesByTag(Container);
}

void ABasePlayer::OnRightLightAttack_Started()
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayTagContainer Container;
    Container.AddTag(Nyota::Ability_RightLightAttack);
    ASC->TryActivateAbilitiesByTag(Container);
}

void ABasePlayer::OnSkill_1_Started()
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayTagContainer Container;
    Container.AddTag(Skill_1_Tag);
    ASC->TryActivateAbilitiesByTag(Container);
}

void ABasePlayer::OnSkill_1_Completed()
{
}

void ABasePlayer::OnSkill_2_Started()
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayTagContainer Container;
    Container.AddTag(Skill_2_Tag);
    ASC->TryActivateAbilitiesByTag(Container);
}

void ABasePlayer::OnSkill_2_Completed()
{
}

void ABasePlayer::OnSkill_3_Started()
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    FGameplayTagContainer Container;
    Container.AddTag(Skill_3_Tag);
    ASC->TryActivateAbilitiesByTag(Container);
}

void ABasePlayer::OnSkill_3_Completed()
{
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
