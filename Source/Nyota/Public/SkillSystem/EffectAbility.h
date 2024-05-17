// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/SphereComponent.h"
#include <atomic>
#include "EffectAbility.generated.h"

class APawnBase;
//typedef void (UEffectAbility::* MontageAnimNotifyDelagateFunPtr)(FString&);

// 定义日志类别
DECLARE_LOG_CATEGORY_EXTERN(SkillSystemLog, Log, All)
/**
 * UEffectAbility 所有作用效果基类
 * 
 * @note 转发所有进入法术场的Effect给具体的目标对象
 */
UCLASS()
class NYOTA_API UEffectAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEffectAbility();

public:

	/*
	* 创建一个球形法术场，附加
	* @param  EffectAbilityObjPossessor 传入拥有的
	* @param Scale 碰撞体缩放
	* @param Offset 碰撞体偏移
	* @param SocketName 附加套接字
	* 
	* @return 是否创建成功
	*/
	static bool CreateSphereEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FVector Scale, FVector Offset, FString SocketName);
	
	/*
	* 创建一个球形法术场移动
	* @param EffectAbilityObjPossessor 传入拥有者
	* @param Scale 碰撞体缩放
	* @param Position 碰撞体放置位置
	* @param MoveDirection 移动方向
	* 
	* @return 直接返回SphereComponent
	*/
	static AActor* CreateSphereEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FVector Scale, FVector Position, FVector MoveDirection);

	/*
	* 创建射线法术场
	* @param EffectAbilityObjPossessor 传入拥有者
	* @param Transform 射线方向，长度，产生位置
	* 
	*/
	static void CreateRayLineEffectAbility(UEffectAbility* EffectAbilityObjPossessor, FTransform& Transform);

private:

	//有目标进入球体法术场范围，这里做统一转发交给指定Effect
	void OnTriggerEnter(APawnBase& AimObj);

public:
	//子类需要覆盖掉，处理具体的法术场作用效果
	//virtual void OnTriggerHandle(USphereComponent Colder);

private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

public:
	//GAS框架加入Cpp回调激活
	virtual void ActivateCppAbility();

	//失活
	virtual void EndCppAbility();

private:
	std::atomic<int> ComponentIndex;
	//拥有EffectAbility的Pawn
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
