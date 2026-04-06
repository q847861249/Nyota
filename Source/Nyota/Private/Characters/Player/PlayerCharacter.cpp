// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/Nyota_PlayerState.h"
#include "Controller/Nyota_PlayerController.h"
#include "UI/HUD/Nyota_HUD.h"
#include "UI/HUD/HUD_Layout.h"
#include "UI/Game/Main/MainUI.h"
#include "UI/HUD/HUD_Layout.h"
APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
    SpringComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringComponent, USpringArmComponent::SocketName);

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    if (!GetAbilitySystemComponent() || !HasAuthority()) return;

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
    SetStartAbilities();
    InitializedAttributes();
}
void APlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    if (!GetAbilitySystemComponent()) return;

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);

    UE_LOG(LogTemp,Warning,TEXT("PlayerState同步"));

    // ANyota_PlayerController* PC = Cast<ANyota_PlayerController>(GetController());
    // if (!PC) return;
  
    // ANyota_HUD* HUD = Cast<ANyota_HUD>(PC->GetHUD());
    // if(!HUD) return;
    // UHUD_Layout* HUD_layout = Cast<UHUD_Layout>(HUD->RootLayout);
    // if(!HUD_layout) return;
    // UMainUI* MainUI = HUD_layout->MainUI;
    // if(!MainUI) return;
    // MainUI->UpdatePlayerList();


}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // Todo: replicated ComboIndex
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
    ANyota_PlayerState* PS = Cast<ANyota_PlayerState>(GetPlayerState());
    if (!PS) return nullptr;

    return PS->GetAbilitySystemComponent();
}