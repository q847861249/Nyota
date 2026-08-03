// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/UI/GA_OpenMap.h"

#include "UI/HUD/Main/OpenMap.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/PlayerController.h"
#include "UI/HUD/Nyota_HUD.h"
#include "CommonUserWidget.h"
#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/HUD/HUD_Layout.h"

void UGA_OpenMap::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    OpenMap();
    // Release Event Listen
    UAbilityTask_WaitInputRelease *WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
    WaitReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputReleaseAgain);
    WaitReleaseTask->ReadyForActivation();
}

void UGA_OpenMap::OnInputReleaseAgain(float TimeWaited)
{
    CloseMap();
    EndAbility( CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false );
}

void UGA_OpenMap::OpenMap()
{
    if(!MapClass) return;
    const FGameplayAbilityActorInfo* CurrentInfo = GetCurrentActorInfo();
    if (!CurrentInfo) return;
    
    APlayerController* PlayerController = CurrentInfo->PlayerController.Get();
    // APlayerController* PlayerController = ActorInfo->PlayerController.Get();
    if(!PlayerController) return;
    ANyota_HUD* NyotaHUD = Cast<ANyota_HUD>(PlayerController->GetHUD());
    if(!NyotaHUD) return;
    UHUD_Layout* HUDLayout = Cast<UHUD_Layout>(NyotaHUD->RootLayout);
    if(!HUDLayout) return;
    UCommonActivatableWidgetStack* MenuStack = HUDLayout->MenuStack;
    if(!MenuStack) return;

    MapWidget = Cast<UOpenMap>(MenuStack->AddWidget(MapClass));
}

void UGA_OpenMap::CloseMap()
{
    MapWidget->DeactivateWidget();
}