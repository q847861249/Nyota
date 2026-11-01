#pragma once

#include "CoreMinimal.h"

/** @brief 抓取期间的四方向输入规则，与相机及技能表现无关。 */
namespace NyotaGrabMovement
{
/**
 * @brief 保留仍被按住的轴；松开后切换到另一轴，同时按下时优先前后。
 * @param Axis X 为左右，Y 为前后。
 * @param PreviousAxis 上次选择：0 无方向，1 左右，2 前后。
 * @return 本次唯一允许的移动轴，始终不会同时启用两个轴。
 */
inline int32 SelectAxis(const FVector2D &Axis, int32 PreviousAxis)
{
    /** 忽略浮点误差造成的零附近输入。 */
    const bool bHorizontal = !FMath::IsNearlyZero(Axis.X);
    const bool bVertical = !FMath::IsNearlyZero(Axis.Y);
    if (PreviousAxis == 1 && bHorizontal)
    {
        return 1;
    }
    if (PreviousAxis == 2 && bVertical)
    {
        return 2;
    }
    return bVertical ? 2 : (bHorizontal ? 1 : 0);
}
}
