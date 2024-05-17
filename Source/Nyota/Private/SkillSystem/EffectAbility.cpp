// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSystem/EffectAbility.h"
#include "Mmo/PawnBase.h"
#include <GameFramework/ProjectileMovementComponent.h>

DEFINE_LOG_CATEGORY(SkillSystemLog)

UEffectAbility::UEffectAbility()
{
	
}

void UEffectAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//将GAS的流程走一遍
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount > 0)
		{
			return;
		}
		this->EndCppAbility();
	}
}

void UEffectAbility::ActivateCppAbility()
{
}

void UEffectAbility::EndCppAbility()
{
}

APawnBase* UEffectAbility::GetOwnerPawnBase()
{
	if (this->OwnerPawnBase == nullptr)
	{
		this->OwnerPawnBase = StaticCast<APawnBase*>(this->GetActorInfo().OwnerActor.Get());
		return this->OwnerPawnBase;
	}
	return this->OwnerPawnBase;
}

bool UEffectAbility::CreateSphereEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FVector Scale, FVector Offset, FString SocketName)
{
	APawnBase* Possessor = EffectAbilityObjPossessor->GetOwnerPawnBase();

	FString IntAsString = FString::FromInt(EffectAbilityObjPossessor->ComponentIndex++);
	IntAsString = "CollisionComponent" + IntAsString;
	FName ComponetIndex = FName(IntAsString.GetCharArray().GetData());
	USphereComponent* CollisionComponent = Possessor->CreateDefaultSubobject<USphereComponent>(ComponetIndex);
	CollisionComponent->SetRelativeScale3D(Scale);
	//CollisionComponent->AttachToComponent
	if (!CollisionComponent)
	{
		UE_LOG(SkillSystemLog, Error, TEXT("UEffectAbility::CreateSphereEffectAbility Error"));
		//UE_LOG(SkillSystem, Error, TEXT("UEffectAbility::CreateSphereEffectAbility Error"）);
		return false;
	}
	EffectAbilityObjPossessor->CollisionComponent = CollisionComponent;
	bool IsAttachSuccess = Possessor->AttachToSocket(CollisionComponent, SocketName, Offset);
	if (!IsAttachSuccess)
	{
		UE_LOG(SkillSystemLog, Error, TEXT("UEffectAbility::CreateSphereEffectAbility Error"));
		return false;
	}

	FOverlapInfo* OverlpInfp = StaticCast<FOverlapInfo*>(FMemory::Malloc(sizeof(FOverlapInfo)));
	
	return true;
}

AActor* UEffectAbility::CreateSphereEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FVector Scale, FVector Position, FVector MoveDirection)
{
	APawnBase* OwnerPawnBase = EffectAbilityObjPossessor->GetOwnerPawnBase();


	//创建发射器Actor
	// 假设你已经有了一个指向当前世界的指针  
	UWorld* World = EffectAbilityObjPossessor->GetWorld();
	FActorSpawnParameters SpawnActorParams;
	SpawnActorParams.Owner = OwnerPawnBase;
	AActor* EmiterActor = World->SpawnActor<AActor>(AActor::StaticClass(), Position, FRotator(0,0,0), SpawnActorParams);// StaticCast<AActor*>(UGameplayStatics::SpawnObject(AActor::StaticClass(), World->Getgamemode));
	FTransform Trans;
	Trans.SetScale3D(Scale);
	Trans.SetLocation(Position);

	//添加球体碰撞
	FTransform SphereTrans;
	SphereTrans.SetScale3D(Scale);
	USphereComponent* SphereComponent = StaticCast<USphereComponent*>(EmiterActor->AddComponentByClass(USphereComponent::StaticClass(), false, SphereTrans, false));
	FScriptDelegate ScriptDelegate;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	//绑定重写父类方法
	ScriptDelegate.BindUFunction(EffectAbilityObjPossessor, "BeginOverlap");

	SphereComponent->OnComponentBeginOverlap.Add(ScriptDelegate);
	SphereComponent->SetVisibility(true);//调试用
	SphereComponent->bHiddenInGame = false;
	EmiterActor->SetActorTransform(Trans);
	FTransform ProjectileTrans;
	UProjectileMovementComponent* ProjectileMovementComponent = StaticCast<UProjectileMovementComponent*>(EmiterActor->AddComponentByClass(UProjectileMovementComponent::StaticClass(), false, ProjectileTrans, false));
	double Speed = MoveDirection.Length();
	//MoveDirection.Normalize();

	ProjectileMovementComponent->Velocity = MoveDirection; //移动方向,这个要设置大点才有效
	ProjectileMovementComponent->InitialSpeed = Speed; //初始速度
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->ProjectileGravityScale = 0;

	return EmiterActor;
}

void UEffectAbility::CreateRayLineEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FTransform& Transform)
{
}

void UEffectAbility::OnTriggerEnter(APawnBase& AimObj)
{
}

void UEffectAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//保持父方法流程正常执行，并加入c++流程
	if (TriggerEventData && bHasBlueprintActivateFromEvent)
	{
		return;
	}
	if (bHasBlueprintActivate)
	{
		this->ActivateCppAbility();
		UE_LOG(SkillSystemLog, Log, TEXT("UEffectAbility::ActivateAbility %s Blueprint Actived"), *this->GetPathName());
	}
	else
	{
		UE_LOG(SkillSystemLog, Warning, TEXT("UEffectAbility::ActivateAbility %s Blueprint Has Not Actived"), *this->GetPathName());
	}

}
