// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileBase.h"

#include "Debug/Debug.h"
#include "Character/NyotaCharacters.h"
#include "Component/NyotaComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Input/NyotaGameplayTags.h"
#include "Net/UnrealNetwork.h"



// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Actor同步
	bReplicates = true;

	//组件初始化
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectCapsule"));
	ProjetileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrjectileMesh"));

	//Component 附合
	ProjetileMesh->SetupAttachment(CapsuleComponent);
	RootComponent = CapsuleComponent;

	//抛射物轨道设置
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->bInitialVelocityInLocalSpace = true;

	//绑定弹道检测当 Overlap 的时候
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::ProjectileBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AProjectileBase::ProjectileEndOverlap);

	//绑定弹道检测当 OnHit 的时候
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::ProjectileOnHit);

	//尝试从 DataAsset 中读取 projectile 基础信息
	InitilizeConfigStruct();

}

bool AProjectileBase::InitilizeConfigStruct()
{
	if(ProjectileConfigFile){
		
		for (auto projectileConfig : ProjectileConfigFile->ProjectileConFig) {

			if (projectileConfig.ProjectileClass == GetClass() ) {

				ProjectileConfigStruct = projectileConfig;

				return true;
			}
		}
	}

	return false;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	//尝试从 DataAsset 中读取 projectile 基础信息
	InitilizeConfigStruct();

	if (ProjectileConfigStruct.ProjectileVisual.SpawnSound) {

		UGameplayStatics::PlaySoundAtLocation(this, ProjectileConfigStruct.ProjectileVisual.SpawnSound,GetActorLocation());

	}

	if (ProjectileConfigStruct.ProjectileVisual.ParticleOnSpawn) {

		UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileConfigStruct.ProjectileVisual.ParticleOnSpawn, GetActorLocation());

	}
}

void AProjectileBase::ProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	//确保instigator 不会进入碰撞检测
	if (OtherActor != GetInstigator()) {
	
		if (ANyotaCharacters* TargetCharacter = Cast<ANyotaCharacters>(OtherActor)) {

			TargetCharacter->NyotaComponent->CurrentActivateProjectileInfo = ProjectileConfigStruct;

			const FNyotaGameplayTags& GamplayTags = FNyotaGameplayTags::Get();

			TargetCharacter->TryActiveAbilityByTag(GamplayTags.State_OnHit);

	
		}
		if (HasAuthority()) PlayEffect_Multicast();
	}
}

void AProjectileBase::ProjectileEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	

}

void AProjectileBase::ProjectileOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetInstigator()) {
	
		if (HasAuthority()) PlayEffect_Multicast();
	}

}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AProjectileBase::PlayEffect_Multicast_Implementation()
{
	if (ProjectileConfigStruct.ProjectileVisual.HitSound) {

		UGameplayStatics::PlaySoundAtLocation(this, ProjectileConfigStruct.ProjectileVisual.HitSound, GetActorLocation());

	}

	if (ProjectileConfigStruct.ProjectileVisual.ParticleOnHit) {

		UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileConfigStruct.ProjectileVisual.ParticleOnHit, GetActorLocation());
	}
	Destroy();

}





