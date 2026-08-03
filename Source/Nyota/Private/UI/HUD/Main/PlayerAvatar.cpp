// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/Main/PlayerAvatar.h"
#include "Components/Image.h"

void UPlayerAvatar::SetAvatar(UTexture2D* AvatarTexture)
{
    AvatarImage->SetBrushFromTexture(AvatarTexture);
}