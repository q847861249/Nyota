// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/MiniMap.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"

void UMiniMap::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry,InDeltaTime);

    if(!CachedPlayerPawn.IsValid())
    {
        CachedPlayerPawn = GetOwningPlayerPawn();
        if(!CachedPlayerPawn.IsValid()) return;
    }

    /**The normalized ratio is obtained by dividing the difference between the player's coordinates
     *  and the world map coordinates by the size of the world map. 
     * Finally, this ratio is multiplied by the size of the small map to obtain the exact mapped coordinates. 
     * Since the offset effect is caused by the movement of the background minimap image, 
     * the result is taken as negative. 
     * */
    FVector WorldPos = CachedPlayerPawn->GetActorLocation();
    //把三维坐标转化为2维
    FVector2D CurrentPlayerLocation(WorldPos.X, WorldPos.Y);
    FVector2D NewLocation = -(CurrentPlayerLocation - LeftTopLocation)/(RightButtomLocation - LeftTopLocation) * ImageSize;
    NewLocation.X += X_Location_offset;
    NewLocation.Y += Y_Location_offset;
    MinimalMapImage->SetRenderTranslation(NewLocation);
    //Set the pointer rotation depend on player rotation
    FRotator PawnRotate = CachedPlayerPawn->GetActorRotation();
    PointerImage->SetRenderTransformAngle(PawnRotate.Yaw);

}
void UMiniMap::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Warning, TEXT("MiniMap"));
}
