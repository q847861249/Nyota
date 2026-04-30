// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WaterBubble.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaterBubbleHit, const FHitResult &, HitResult);

// TODO: 这里同 AWaterBall 类内容基本一致，考虑抽成一个，使用蓝图继承同一个

UCLASS()
class PROTOTYPE_API AWaterBubble : public AActor
{
    GENERATED_BODY()

public:
    AWaterBubble();

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintAssignable)
    FOnWaterBubbleHit OnWaterBubbleHit;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraComponent> FlightVFX;

    UPROPERTY(EditDefaultsOnly, Category = "WaterBubble")
    TObjectPtr<UNiagaraSystem> HitVFX;

    UPROPERTY(EditDefaultsOnly, Category = "WaterBubble")
    float MaxRange;

    FVector SpawnLocation;

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
    void PopAndDestroy(const FVector &Location);
};
