// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/BlueprintUtilsLibrary.h"

EHitDirection UBlueprintUtilsLibrary::GetHitDirection(const FVector &TargetForward, const FVector &ToInstigator)
{
    // ==============================
    // 1. 使用点积（DotProduct）判断前后方向
    // ==============================

    // 点积用于判断两个向量的夹角
    // Dot 值范围：[-1, 1]
    //
    // Dot ≈ 1   → 同方向（正前方）
    // Dot ≈ 0   → 垂直（左右）
    // Dot ≈ -1  → 反方向（正后方）
    const float Dot = FVector::DotProduct(TargetForward, ToInstigator);

    // ==============================
    // 2. 判断是否来自背后
    // ==============================

    // Dot < -0.5
    // 表示夹角 > 120°
    // 说明攻击来自角色背后
    if (Dot < -0.5)
    {
        return EHitDirection::Back;
    }

    // ==============================
    // 3. 判断是否来自左右
    // ==============================

    // -0.5 <= Dot < 0.5
    // 表示夹角在 60° ~ 120°
    // 说明攻击来自侧面
    if (Dot < 0.5)
    {
        // 使用叉积（CrossProduct）判断左右方向
        // Cross.Z 的正负决定左右
        const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);

        // Cross.Z < 0
        // 表示攻击来自左侧
        if (Cross.Z < 0.f)
        {
            return EHitDirection::Left;
        }

        // 否则来自右侧
        return EHitDirection::Right;
    }

    // ==============================
    // 4. 默认来自正前方
    // ==============================

    // Dot >= 0.5
    // 表示夹角 < 60°
    // 说明攻击来自前方
    return EHitDirection::Forward;
}

FName UBlueprintUtilsLibrary::GetHitDirectionName(const EHitDirection &HitDirection)
{
    switch (HitDirection)
    {
    case EHitDirection::Left: {
        return FName("Left");
    }
    case EHitDirection::Right: {
        return FName("Right");
    }
    case EHitDirection::Forward: {
        return FName("Forward");
    }
    case EHitDirection::Back: {
        return FName("Back");
    }
    default: {
        return FName("None");
    }
    }
}