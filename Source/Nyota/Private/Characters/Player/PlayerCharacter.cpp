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
#include "AbilitySystemBlueprintLibrary.h"
#include "Tags/Nyota_Tag.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
    SpringComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringComponent, USpringArmComponent::SocketName);

}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
    }
}

void APlayerCharacter::PossessedBy(AController *NewController) 
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
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // Todo: replicated ComboIndex
    DOREPLIFETIME_CONDITION_NOTIFY(APlayerCharacter, CurrentComboIndex,COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(APlayerCharacter, bCanCombo,COND_None, REPNOTIFY_Always);
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
    ANyota_PlayerState* PS = Cast<ANyota_PlayerState>(GetPlayerState());
    if (!PS) return nullptr;

    return PS->GetAbilitySystemComponent();
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult)
{
    // Check if the actor has specific tag
    if (OtherActor && OtherActor->ActorHasTag(FName("Coin")))
    {
        FGameplayEventData Payload;
        Payload.Instigator = this;
        Payload.Target = OtherActor;
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,Nyota::Event::PickUp,Payload);
    }
    
}


int32 APlayerCharacter::NextComboIndex()
{
    CurrentComboIndex = (CurrentComboIndex + 1) % MaxComboIndex;

    return CurrentComboIndex;
}

int32 APlayerCharacter::GetComboIndex() const
{
    return CurrentComboIndex;
}

void APlayerCharacter::SetMaxComboIndex(int32 IndexNum)
{
    MaxComboIndex = IndexNum;
}

void APlayerCharacter::ResetComboIndex(float ResetTime)
{
    if(HasAuthority())
    {
        GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle,
            [this]() 
            {
                this->CurrentComboIndex = -1;
            }, ResetTime, false);
    }
}