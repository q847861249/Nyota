#pragma once

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
    Crab = 0 UMETA(DisplayName = "PangXie"),
    WildBoar = 1 UMETA(DisplayName = "YeZhu"),
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
    Left,
    Right,
    Forward,
    Back
};