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
	//��GAS��������һ��
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
		//UE_LOG(SkillSystem, Error, TEXT("UEffectAbility::CreateSphereEffectAbility Error"��);
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


	//����������Actor
	// �������Ѿ�����һ��ָ��ǰ�����ָ��  
	UWorld* World = EffectAbilityObjPossessor->GetWorld();
	FActorSpawnParameters SpawnActorParams;
	SpawnActorParams.Owner = OwnerPawnBase;
	AActor* EmiterActor = World->SpawnActor<AActor>(AActor::StaticClass(), Position, FRotator(0,0,0), SpawnActorParams);// StaticCast<AActor*>(UGameplayStatics::SpawnObject(AActor::StaticClass(), World->Getgamemode));
	FTransform Trans;
	Trans.SetScale3D(Scale);
	Trans.SetLocation(Position);

	//���������ײ
	FTransform SphereTrans;
	SphereTrans.SetScale3D(Scale);
	USphereComponent* SphereComponent = StaticCast<USphereComponent*>(EmiterActor->AddComponentByClass(USphereComponent::StaticClass(), false, SphereTrans, false));
	FScriptDelegate ScriptDelegate;

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	//����д���෽��
	ScriptDelegate.BindUFunction(EffectAbilityObjPossessor, "BeginOverlap");

	SphereComponent->OnComponentBeginOverlap.Add(ScriptDelegate);
	SphereComponent->SetVisibility(true);//������
	SphereComponent->bHiddenInGame = false;
	EmiterActor->SetActorTransform(Trans);
	FTransform ProjectileTrans;
	UProjectileMovementComponent* ProjectileMovementComponent = StaticCast<UProjectileMovementComponent*>(EmiterActor->AddComponentByClass(UProjectileMovementComponent::StaticClass(), false, ProjectileTrans, false));
	double Speed = MoveDirection.Length();
	//MoveDirection.Normalize();

	ProjectileMovementComponent->Velocity = MoveDirection; //�ƶ�����,���Ҫ���ô�����Ч
	ProjectileMovementComponent->InitialSpeed = Speed; //��ʼ�ٶ�
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
	//���ָ�������������ִ�У�������c++����
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
