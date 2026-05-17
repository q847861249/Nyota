// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/BlueprintUtilsLibrary.h"

#include "Engine/OverlapResult.h"

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

TArray<AActor *> UBlueprintUtilsLibrary::HitBoxOverlapTest(
    UObject *WorldContextObject, AActor *Instigator, float HitBoxRadius, const FVector &ForwardVector,
    float HitBoxElevationOffset, bool bDrawDebugs
)
{
    UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!IsValid(World))
    {
        return TArray<AActor *>();
    }

    // ==============================
    // 1. 设置忽略 Actor（通常忽略自己）
    // ==============================

    TArray<AActor *> ActorsToIgnore;

    // 忽略当前技能拥有者（否则会打到自己）
    ActorsToIgnore.Add(Instigator);

    // 创建查询参数
    FCollisionQueryParams QueryParams;

    // 将忽略列表加入查询参数
    QueryParams.AddIgnoredActors(ActorsToIgnore);

    // ==============================
    // 2. 设置碰撞响应规则
    // ==============================

    FCollisionResponseParams ResponseParams;

    // 默认忽略所有碰撞通道
    ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);

    // 只检测 Pawn（角色）
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

    // ==============================
    // 3. 创建球形检测区域（Sphere HitBox）
    // ==============================

    // 根据半径创建球形碰撞体
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(HitBoxRadius);

    // ==============================
    // 4. 计算 HitBox 位置（角色前方）
    // ==============================

    // 获取角色前方向，并乘以前移距离，如果未设置使用默认获取到的值
    const FVector Forward = ForwardVector;

    // 最终 HitBox 位置：
    // 角色位置 + 前移 + 高度偏移
    const FVector ElevationOffset = FVector(0.0f, 0.0f, HitBoxElevationOffset);
    const FVector HitBoxLocation = Instigator->GetActorLocation() + Forward + ElevationOffset;

    // ==============================
    // 5. 执行 Overlap 检测
    // ==============================

    // 用于存储检测到的结果
    TArray<FOverlapResult> OverlapResults;

    // 执行多目标重叠检测（Sphere Overlap）
    World->OverlapMultiByChannel(
        OverlapResults,  // 输出：命中的结果列表
        HitBoxLocation,  // 检测位置
        FQuat::Identity, // 旋转（Sphere 不需要旋转）
        ECC_Visibility,  // 使用的检测通道
        CollisionShape,  // 碰撞体形状（Sphere）
        QueryParams,     // 查询参数（忽略 Actor）
        ResponseParams   // 碰撞响应规则
    );

    TArray<AActor *> HitActors;

    for (const FOverlapResult &Result : OverlapResults)
    {
        if (!IsValid(Result.GetActor()))
        {
            continue;
        }

        HitActors.AddUnique(Result.GetActor());
    }

    // ==============================
    // 6. Debug 绘制（可选）
    // ==============================

    if (bDrawDebugs)
    {
        DrawDebugInformation(WorldContextObject, OverlapResults, HitBoxLocation, HitBoxRadius);
    }

    return HitActors;
}

void UBlueprintUtilsLibrary::DrawDebugInformation(
    UObject *WorldContextObject, const TArray<FOverlapResult> &OverlapResults, const FVector &HitBoxLocation,
    float HitBoxRadius
)
{
    UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!IsValid(World))
    {
        return;
    }

    // 绘制检测球体（红色）
    DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

    // 绘制命中的 Actor 位置
    for (const FOverlapResult &Result : OverlapResults)
    {
        if (!IsValid(Result.GetActor()))
        {
            continue;
        }

        // 获取命中 Actor 位置
        FVector ActorLocation = Result.GetActor()->GetActorLocation();

        // 在 Actor 位置画球
        DrawDebugSphere(World, ActorLocation, 30.f, 10, FColor::Green, false, 3.f);
    }
}