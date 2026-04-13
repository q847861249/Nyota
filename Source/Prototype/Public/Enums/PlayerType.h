#pragma once

UENUM(BlueprintType)
enum class EPlayerType : uint8
{
    PangXie = 0 UMETA(DisplayName = "PangXie"),
    YeZhu = 1 UMETA(DisplayName = "YeZhu"),
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
    Left,
    Right,
    Forward,
    Back
};