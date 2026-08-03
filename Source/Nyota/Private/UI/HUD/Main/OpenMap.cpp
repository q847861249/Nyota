// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/OpenMap.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "UI/HUD/Main/LootPointIcon.h"
#include "Actor/LootPoint.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Character/BasePlayer.h"
#include "DataAsset/PlayerCharacterDataAsset.h"
#include "UI/HUD/Main/PlayerAvatar.h"

void UOpenMap::NativeOnActivated()
{
    Super::NativeOnActivated();
    RefreshLootPoint();
    RefreshPlayer();
}
void UOpenMap::NativeOnDeactivated()
{
    Super::NativeOnDeactivated();
    if(!IsValid(LootPointPanel)) return;
    if(!IsValid(LootPointIconClass)) return;
    LootPointPanel->ClearChildren();
    LootPointTMap.Empty();
    PlayerAvatar = nullptr;

}
void UOpenMap::NativeTick(const FGeometry& MyGeometry,float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    const FVector2D MapSize = LootPointPanel->GetCachedGeometry().GetLocalSize();
    for (auto It = LootPointTMap.CreateIterator(); It; ++It)
    {
        ALootPoint* LootPoint = It.Key();
        ULootPointIcon* Icon = It.Value();
        UCanvasPanelSlot* IconSlot = Cast<UCanvasPanelSlot>(Icon->Slot);
        const FVector2D Position = GetIconMapPosition(LootPoint->GetActorLocation(), MapSize);
        IconSlot->SetPosition(Position);
    }
    if(!IsValid(PlayerAvatar)) return;
    UCanvasPanelSlot* AvatarSlot = Cast<UCanvasPanelSlot>(PlayerAvatar->Slot);
    if(!AvatarSlot) return;
    const FVector2D AvatarPosition = GetIconMapPosition(GetOwningPlayerPawn()->GetActorLocation(), MapSize);
    AvatarSlot->SetPosition(AvatarPosition);
}
void UOpenMap::RefreshLootPoint()
{
    if(!IsValid(LootPointPanel)) return;
    if(!IsValid(LootPointIconClass)) return;
    // LootPointPanel->ClearChildren();
    // LootPointTMap.Empty();
    TArray<AActor*> LootPoinActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),ALootPoint::StaticClass(),LootPoinActors);
    for(AActor* Actor: LootPoinActors)
    {
        ALootPoint* LootPointActor = Cast<ALootPoint>(Actor);
        if(!IsValid(LootPointActor)) continue;
        ULootPointIcon* LootPointIcon = CreateWidget<ULootPointIcon>(GetOwningPlayer(),LootPointIconClass);
        if(!IsValid(LootPointIcon)) continue;
        UCanvasPanelSlot* IconSlot = LootPointPanel->AddChildToCanvas(LootPointIcon);
        if (!IconSlot) continue;       
        IconSlot->SetAutoSize(true);
        IconSlot->SetAlignment(FVector2D(0.5f, 0.5f));
        LootPointTMap.Add(LootPointActor, LootPointIcon);
    }
}
void UOpenMap::RefreshPlayer()
{
    if(!IsValid(LootPointPanel)) return;
    if(!IsValid(LootPointIconClass)) return;
    if(!IsValid(PlayerAvatarClass)) return;
    if(!IsValid(PlayerAvatar))
    {
        PlayerAvatar = CreateWidget<UPlayerAvatar>(GetOwningPlayer(),PlayerAvatarClass);
        if(!PlayerAvatar) return;        

        if(!DefaultPlayerAvatarTexture)
        {
            ABasePlayer* Player = Cast<ABasePlayer>(GetOwningPlayerPawn());
            if(!Player) return;
            UPlayerCharacterDataAsset* PlayerDA = Player->GetConfig();
            if(!IsValid(PlayerDA)) return;
            PlayerAvatar->SetAvatar(PlayerDA->Avatar);             
        }
        else
        {
            PlayerAvatar->SetAvatar(DefaultPlayerAvatarTexture);
        }

        UCanvasPanelSlot* AvatarSlot = LootPointPanel->AddChildToCanvas(PlayerAvatar);
        if (!AvatarSlot) return;
        AvatarSlot->SetAutoSize(false);
        AvatarSlot->SetSize(ImageSize);
        AvatarSlot->SetAlignment(FVector2D(0.5f, 0.5f));   
    }

}

FVector2D UOpenMap::GetIconMapPosition(const FVector& TargetPosition,const FVector2D& MapSize)
{
    FVector2D Target2DPos(TargetPosition.X,TargetPosition.Y);
    FVector2D MappingLocation = (Target2DPos - LeftTopLocation)/(RightButtomLocation - LeftTopLocation) * MapSize;
    return MappingLocation;
}