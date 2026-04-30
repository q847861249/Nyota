// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/BaseProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile::ABaseProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    // 碰撞球
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(20.f);
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = CollisionComponent;

    // 飞行特效挂在碰撞球上
    FlightVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlightVFX"));
    FlightVFX->SetupAttachment(RootComponent);
    FlightVFX->bAutoActivate = true;

    // 飞行组件
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovementComponent->InitialSpeed = 2000.f;
    ProjectileMovementComponent->MaxSpeed = 2000.f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f; // 不受重力影响
}

void ABaseProjectile::BeginPlay()
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

void ABaseProjectile::OnHit(
    UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse,
    const FHitResult &Hit
)
{
    if (OtherActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Projectile Hit: %s"), *OtherActor->GetName());
    }

    // 跳过自己和发射者
    if (!OtherActor || OtherActor == this || OtherActor == GetInstigator())
    {
        return;
    }

    // 通知外部更新
    OnProjectileHit.Broadcast(Hit);

    ExplodeAndDestroy(Hit.ImpactPoint);
}

void ABaseProjectile::ExplodeAndDestroy(const FVector &Location)
{
    // 播放爆炸特效
    if (ExplosionVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX, Location);
    }

    Destroy();
}

void ABaseProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 超出最大距离就销毁
    float DistanceTraveled = FVector::Dist(SpawnLocation, GetActorLocation());
    if (DistanceTraveled >= MaxRange)
    {
        ExplodeAndDestroy(GetActorLocation());
    }
}
