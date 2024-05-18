// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// 修改：添加引用
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Event/EventTrigger.h"
#include "SkillSystem/EffectAbility.h"
#include <concepts>
#include "PawnBase.generated.h"

// 定义日志类别
DECLARE_LOG_CATEGORY_EXTERN(MmoFrameLog, Log, All)

DECLARE_MULTICAST_DELEGATE_OneParam(FMontageAnimNotifyDelagate, FString)

// 定义一个通用的成员函数指针模板
template<typename ReturnType, typename ClassType, typename... Args>
//using ClassType1 = std::is_base_of_v<APawnBase, ClassType>;
using MemberFunctionPtr = ReturnType(ClassType::*)(Args...);

template<typename ReturnType, typename ClassType, typename... Args>
struct FAnimNotifyDelagateInfo
{
public:
	FString NotifyName;
	float NotifyTime;
	MemberFunctionPtr<ReturnType, ClassType, Args...> MontageAnimNotifyFunPtr;
	FMontageAnimNotifyDelagate* MontageAnimNotifyDelagate;
};

UCLASS()
class NYOTA_API APawnBase : public APawn,
	public IAbilitySystemInterface //修改：继承接口
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 修改：申明ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	// 修改：实现接口方法
	UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	// 修改：声明Ability数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> MyAbilities;

	//事件触发器
	EventTrigger<FString>* EventTriggerObj;

	//UFUNCTION()
	void OnTimerNotify(FString NotifyName);

	//UFUNCTION()
	void OnStatusNotify(FString NotifyName);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//动画通知委托数组,动画播放完释放掉对应元素内存,1动画->n通知

	TMap<FString, TArray<FAnimNotifyDelagateInfo<void, UObject, FString*>*>> MontAnmNotifyDelMap;

	/*
	* @ note 1个NotifyName-> 1个动画 ,NotifyName= AnimName_NotifyName
	*/
	FAnimNotifyDelagateInfo<void, UObject, FString*>* FindFAnimNotifyDelagateInfo(FString& MontaqeAnimNotifyName);

	template<typename ReturnType, typename ClassType, typename... Args>
	void PlayMontageAnim(USkeletalMeshComponent* AnimSkeletal, FString& AnimName, FString& NotifyName);

public:
	/*
	* 绑定给对象绑定动画通知
	* @param BindObj 需要通知对象
	* @param AnimName 绑定通知的动画
	* @param AnimNotifyDelagateInfo 绑定通知的信息
	*/
	template<typename ReturnType, typename ClassType, typename... Args>
	void BindMontageAnimNotify(UObject* BindObj, FString& AnimName, FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* AnimNotifyDelagateInfo);

	/*
	* 播放蒙太奇动画接口
	* @param AnimName 播放的动画名称，自动查找匹配的骨骼，如Pawn1_Anim1
	* @param NotifyName 用于通知
	* @param NotifyTime 通知的时间
	* @return 播放是否成功
	*/
	template<typename ReturnType, typename ClassType, typename... Args>
	bool PlayMontageAnim(UObject* PlayAnimAblyObj, MemberFunctionPtr<ReturnType, ClassType, Args...> MontageAimnNotifyDelaFunPtr, FString& AnimName, FString& NotifyName,float NotifyTime);
public:
	//获取所有动画骨骼,1Anim->1SkeletalMesh
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, USkeletalMeshComponent*> AnimSkeletalMeshMap;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	//AttachToSocket(CollisionComponent, SocketName, Offset)
	bool AttachToSocket(USceneComponent* SceneComponent, FString SocketName, FVector Offset);
	
public:

	UFUNCTION()
	void OnAnimNotifyOneParam(FString& NotifyName);

};
