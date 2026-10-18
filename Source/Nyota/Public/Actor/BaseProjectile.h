// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileHit, const FHitResult &, Hit);

class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class NYOTA_API ABaseProjectile : public AActor
{
    GENERATED_BODY()

public:
    ABaseProjectile();

    virtual void Tick(float DeltaTime) override;

    /**
     * @brief 按指定初速度与重力发射投射物（重力 0 = 直线，1 = 与角色一致的抛物线）
     * @param LaunchVelocity 初始速度
     * @param GravityScale 重力缩放
     */
    void Launch(const FVector &LaunchVelocity, float GravityScale);

    UPROPERTY(BlueprintAssignable)
    FOnProjectileHit OnProjectileHit;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> CollisionComponent; // 碰撞组件

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent; // 飞行组件

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraComponent> FlightVFX; // 飞行特效

    UPROPERTY(EditDefaultsOnly, Category = "WaterBall")
    TObjectPtr<UNiagaraSystem> ExplosionVFX; // 爆炸特效

    UPROPERTY(EditDefaultsOnly, Category = "WaterBall")
    float MaxRange = 3000.f; // 最大飞行距离

    FVector SpawnLocation; // 记录出生位置

    /**
     * @brief 处理命中
     *
     * @param HitComp
     * @param OtherActor
     * @param OtherComp
     * @param NormalImpulse
     * @param Hit
     */
    UFUNCTION()
    void OnHit(
        UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse,
        const FHitResult &Hit
    );

    /**
     * @brief 爆炸并摧毁
     *
     * @param Location
     */
    void ExplodeAndDestroy(const FVector &Location);
};
