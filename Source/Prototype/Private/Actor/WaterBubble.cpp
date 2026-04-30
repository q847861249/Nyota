// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/WaterBubble.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AWaterBubble::AWaterBubble()
{
    PrimaryActorTick.bCanEverTick = true;

    // 碰撞
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(15.f);
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = CollisionComponent;

    // 飞行特效
    FlightVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlightVFX"));
    FlightVFX->SetupAttachment(RootComponent);
    FlightVFX->bAutoActivate = true;

    // 飞行组件
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovementComponent->InitialSpeed = 1000.f;
    ProjectileMovementComponent->MaxSpeed = 1000.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.2f; // 泡泡有一点点重力，飘起来的感觉
}

void AWaterBubble::BeginPlay()
{
    Super::BeginPlay();

    // 记录起始位置
    SpawnLocation = GetActorLocation();

    // 忽略发射者的碰撞
    if (GetInstigator())
    {
        CollisionComponent->MoveIgnoreActors.Add(GetInstigator());

        // 设置发射方向
        if (IsValid(Cast<ABaseCharacter>(GetInstigator())))
        {
            const FVector ForwardVector = Cast<ABaseCharacter>(GetInstigator())->GetForwardDirection();
            ProjectileMovementComponent->Velocity = ForwardVector * ProjectileMovementComponent->InitialSpeed;
        }

        CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
    }
}

void AWaterBubble::OnHit(
    UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse,
    const FHitResult &Hit
)
{
    if (OtherActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("WaterBubble Hit: %s"), *OtherActor->GetName());
    }

    if (!OtherActor || OtherActor == this || OtherActor == GetInstigator())
    {
        return;
    }

    OnWaterBubbleHit.Broadcast(Hit);

    PopAndDestroy(Hit.ImpactPoint);
}

void AWaterBubble::PopAndDestroy(const FVector &Location)
{
    // 播放爆炸特效
    if (HitVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, Location);
    }

    Destroy();
}

void AWaterBubble::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float Distance = FVector::Dist(SpawnLocation, GetActorLocation());
    if (Distance >= MaxRange)
    {
        PopAndDestroy(GetActorLocation());
    }
}
