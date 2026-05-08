// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "BaseEnemyWildBoar.generated.h"

UCLASS()
class PROTOTYPE_API ABaseEnemyWildBoar : public ABaseEnemy
{
    GENERATED_BODY()

public:
    /**
     * @brief 抓取事件
     * @param Grabber 抓取来源
     */
    UFUNCTION(BlueprintCallable)
    void OnGrabbed(ACharacter *Grabber);

    /**
     * @brief 取消抓取
     */
    UFUNCTION(BlueprintCallable)
    void OnReleased();

    /**
     * @brief 摔出
     * @param Direction 丢出方向
     * @param Force 丢出的力
     */
    UFUNCTION(BlueprintCallable)
    void OnThrown(FVector Direction, float Force);

    /**
     * @brief 落地
     * @param Hit
     */
    UFUNCTION()
    void OnBoarLanded(const FHitResult &Hit);

    UFUNCTION(BlueprintCallable)
    void OnSlammed(int32 SlamCount);

    UFUNCTION(BlueprintImplementableEvent)
    void OnGrabbed_Visual(); // 蓝图里播放被抓特效

    UFUNCTION(BlueprintImplementableEvent)
    void OnSlammed_Visual(); // 蓝图里播放砸地受击特效

    UFUNCTION(BlueprintImplementableEvent)
    void OnThrown_Visual(); // 蓝图里播放被摔特效

    UFUNCTION(BlueprintImplementableEvent)
    void OnLanded_Visual(); // 蓝图里播放落地特效

private:
    bool bIsGrabbed = false;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | WildBoar")
    float ThrownDamage;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | WildBoar")
    float ThrownDamageRadius;
};
