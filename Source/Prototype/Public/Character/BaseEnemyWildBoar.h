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

    bool bIsGrabbed = false;

    UPROPERTY(EditDefaultsOnly, Category = "WildBoar")
    float ThrownDamage;

    UPROPERTY(EditDefaultsOnly, Category = "WildBoar")
    float ThrownDamageRadius;
};
