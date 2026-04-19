// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/Main/MiniMap.h"
#include "GameFramework/Pawn.h"
#include "GameState/Nyota_GameState.h"
#include "GameFramework/PlayerState.h"
void UMiniMap::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry,InDeltaTime);

    if(!CachedPlayerPawn.IsValid())
    {
        CachedPlayerPawn = GetOwningPlayerPawn();
        if(!CachedPlayerPawn.IsValid()) return;
    }

    FVector WorldPos = CachedPlayerPawn->GetActorLocation();
    FVector2D CurrentPlayerLocation(WorldPos.X, WorldPos.Y);
    FVector2D NewLocation = -(CurrentPlayerLocation - LeftTopLocation)/(RightButtomLocation - LeftTopLocation) * ImageSize;
    NewLocation.X += X_Location_offset;
    NewLocation.Y += Y_Location_offset;
    MinimalMapImage->SetRenderTranslation(NewLocation);
    // UE_LOG(LogTemp,Warning,TEXT("X:%f,Y:%f"),NewLocation.X,NewLocation.Y);


    FRotator PawnRotate = CachedPlayerPawn->GetActorRotation();
    PointerImage->SetRenderTransformAngle(PawnRotate.Yaw);

}
void UMiniMap::NativeConstruct()
{
    Super::NativeConstruct();

}

