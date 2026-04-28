// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterBall.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class PROTOTYPE_API AWaterBall : public AActor
{
    GENERATED_BODY()

public:
    AWaterBall();

    virtual void Tick(float DeltaTime) override;

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
    float Damage = 30.f; // 伤害

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
