// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/SphereComponent.h"
#include <atomic>
#include "EffectAbility.generated.h"

class APawnBase;
//typedef void (UEffectAbility::* MontageAnimNotifyDelagateFunPtr)(FString&);

// ������־���
DECLARE_LOG_CATEGORY_EXTERN(SkillSystemLog, Log, All)
/**
 * UEffectAbility ��������Ч������
 * 
 * @note ת�����н��뷨������Effect�������Ŀ�����
 */
UCLASS()
class NYOTA_API UEffectAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEffectAbility();

public:

	/*
	* ����һ�����η�����������
	* @param  EffectAbilityObjPossessor ����ӵ�е�
	* @param Scale ��ײ������
	* @param Offset ��ײ��ƫ��
	* @param SocketName �����׽���
	* 
	* @return �Ƿ񴴽��ɹ�
	*/
	static bool CreateSphereEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FVector Scale, FVector Offset, FString SocketName);
	
	/*
	* ����һ�����η������ƶ�
	* @param EffectAbilityObjPossessor ����ӵ����
	* @param Scale ��ײ������
	* @param Position ��ײ�����λ��
	* @param MoveDirection �ƶ�����
	* 
	* @return ֱ�ӷ���SphereComponent
	*/
	static AActor* CreateSphereEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FVector Scale, FVector Position, FVector MoveDirection);

	/*
	* �������߷�����
	* @param EffectAbilityObjPossessor ����ӵ����
	* @param Transform ���߷��򣬳��ȣ�����λ��
	* 
	*/
	static void CreateRayLineEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FTransform& Transform);

private:

	//��Ŀ��������巨������Χ��������ͳһת������ָ��Effect
	void OnTriggerEnter(APawnBase& AimObj);

public:
	//������Ҫ���ǵ����������ķ���������Ч��
	//virtual void OnTriggerHandle(USphereComponent Colder);

private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

public:
	//GAS��ܼ���Cpp�ص�����
	virtual void ActivateCppAbility();

	//ʧ��
	virtual void EndCppAbility();

private:
	std::atomic<int> ComponentIndex;
	//ӵ��EffectAbility��Pawn
	APawnBase* OwnerPawnBase;


public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> MoveActor;

	USphereComponent* CollisionComponent;

	UFUNCTION(BlueprintCallable)
	APawnBase* GetOwnerPawnBase();
public:

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
