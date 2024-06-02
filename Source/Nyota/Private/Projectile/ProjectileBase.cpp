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

	//Actorͬ��
	bReplicates = true;

	//�����ʼ��
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DetectCapsule"));
	ProjetileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrjectileMesh"));

	//Component ����
	ProjetileMesh->SetupAttachment(CapsuleComponent);
	RootComponent = CapsuleComponent;

	//������������
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->bInitialVelocityInLocalSpace = true;

	//�󶨵�����⵱ Overlap ��ʱ��
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::ProjectileBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AProjectileBase::ProjectileEndOverlap);

	//�󶨵�����⵱ OnHit ��ʱ��
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::ProjectileOnHit);

	//���Դ� DataAsset �ж�ȡ projectile ������Ϣ
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

	//���Դ� DataAsset �ж�ȡ projectile ������Ϣ
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
	//ȷ��instigator ���������ײ���
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





