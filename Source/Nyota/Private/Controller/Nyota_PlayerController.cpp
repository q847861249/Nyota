// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Nyota_PlayerController.h"
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Tags/Nyota_Tag.h"
void ANyota_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // get enhanced input system
    // UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if(!Subsystem) return;
    // add input mapping context;
    if(MovingMappingContext)
    {
        Subsystem->AddMappingContext(MovingMappingContext,0);
    }

    if(AbilitiesMappingContext)
    {
        Subsystem->AddMappingContext(AbilitiesMappingContext,0);
    }

    if(GameplayMappingContext)
    {
        Subsystem->AddMappingContext(GameplayMappingContext,0);
    }

    if(!UIMappingContext)
    {
        // Subsystem->AddMappingContext(UIMappingContext,0);
        UE_LOG(LogTemp,Warning,TEXT("Don't set UIMappingContext on Blureprint"));
    }
    // get enhanced input component
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    if(!EnhancedInputComponent) return;
    // bind callback functions to input actions
    EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ANyota_PlayerController::Move);
    EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Completed,this,&ANyota_PlayerController::StopMove);
    EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&ANyota_PlayerController::StartJump);
    EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&ANyota_PlayerController::StopJump);
    EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ANyota_PlayerController::Look);
    EnhancedInputComponent->BindAction(PrimaryAction,ETriggerEvent::Started,this,&ANyota_PlayerController::Primary);
    EnhancedInputComponent->BindAction(QAction,ETriggerEvent::Started,this,&ANyota_PlayerController::QAbility);
    EnhancedInputComponent->BindAction(EAction,ETriggerEvent::Started,this,&ANyota_PlayerController::EAbility);
    EnhancedInputComponent->BindAction(RAction,ETriggerEvent::Started,this,&ANyota_PlayerController::RAbility);
}

void ANyota_PlayerController::ActivateAbilityByTag(const FGameplayTag& AbilityTag)
{
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
    if(!ASC) return;
    FGameplayTagContainer TagContainer;
    TagContainer.AddTag(AbilityTag);
    ASC->TryActivateAbilitiesByTag(TagContainer);
}

void ANyota_PlayerController::StopMove()
{

}

void ANyota_PlayerController::Move(const FInputActionValue& Value)
{
    if(!GetPawn()) return;
    //get movement input value from the input action value
    const FVector2D MoveValue = Value.Get<FVector2D>();
    //get Yaw rotation of the controller
    const FRotator YawRotation(0,GetControlRotation().Yaw,0);
    // get forward and right direction of the controller
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // add movement input to the pawn
    GetPawn()->AddMovementInput(ForwardDirection,MoveValue.Y);
    GetPawn()->AddMovementInput(RightDirection,MoveValue.X);
    // Using GE to cancle Ability
    // UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
    // if(!ASC) return;
    // FGameplayTagContainer TagsToCancel;
    // TagsToCancel.AddTag(SR_Tags::Abilities::Player::Primary); 
    // ASC->CancelAbilities(&TagsToCancel);

}

void ANyota_PlayerController::StartJump()
{
    if(!GetCharacter()) return;
    GetCharacter()->Jump(); // call the jump function of the character movement component
}

void ANyota_PlayerController::StopJump()
{
    if(!GetCharacter()) return;
    GetCharacter()->StopJumping();
}

void ANyota_PlayerController::Look(const FInputActionValue& Value)
{
    if(!GetPawn()) return;

    const FVector2D LookValue = Value.Get<FVector2D>();
    AddYawInput(LookValue.X);
    AddPitchInput(LookValue.Y);
}

void ANyota_PlayerController::Primary()
{
    ActivateAbilityByTag(Nyota::Ability::Primary);
}

void ANyota_PlayerController::QAbility()
{
    ActivateAbilityByTag(Nyota::Ability::Player::Q);
}

void ANyota_PlayerController::EAbility()
{
    ActivateAbilityByTag(Nyota::Ability::Player::E);
}

void ANyota_PlayerController::RAbility()
{
    ActivateAbilityByTag(Nyota::Ability::Player::R);
}