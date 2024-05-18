// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// �޸ģ��������
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Event/EventTrigger.h"
#include "SkillSystem/EffectAbility.h"
#include <concepts>
#include "PawnBase.generated.h"

// ������־���
DECLARE_LOG_CATEGORY_EXTERN(MmoFrameLog, Log, All)

DECLARE_MULTICAST_DELEGATE_OneParam(FMontageAnimNotifyDelagate, FString)

// ����һ��ͨ�õĳ�Ա����ָ��ģ��
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
	public IAbilitySystemInterface //�޸ģ��̳нӿ�
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// �޸ģ�����ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	// �޸ģ�ʵ�ֽӿڷ���
	UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	// �޸ģ�����Ability����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> MyAbilities;

	//�¼�������
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

	//����֪ͨί������,�����������ͷŵ���ӦԪ���ڴ�,1����->n֪ͨ

	TMap<FString, TArray<FAnimNotifyDelagateInfo<void, UObject, FString*>*>> MontAnmNotifyDelMap;

	/*
	* @ note 1��NotifyName-> 1������ ,NotifyName= AnimName_NotifyName
	*/
	FAnimNotifyDelagateInfo<void, UObject, FString*>* FindFAnimNotifyDelagateInfo(FString& MontaqeAnimNotifyName);

	template<typename ReturnType, typename ClassType, typename... Args>
	void PlayMontageAnim(USkeletalMeshComponent* AnimSkeletal, FString& AnimName, FString& NotifyName);

public:
	/*
	* �󶨸�����󶨶���֪ͨ
	* @param BindObj ��Ҫ֪ͨ����
	* @param AnimName ��֪ͨ�Ķ���
	* @param AnimNotifyDelagateInfo ��֪ͨ����Ϣ
	*/
	template<typename ReturnType, typename ClassType, typename... Args>
	void BindMontageAnimNotify(UObject* BindObj, FString& AnimName, FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* AnimNotifyDelagateInfo);

	/*
	* ������̫�涯���ӿ�
	* @param AnimName ���ŵĶ������ƣ��Զ�����ƥ��Ĺ�������Pawn1_Anim1
	* @param NotifyName ����֪ͨ
	* @param NotifyTime ֪ͨ��ʱ��
	* @return �����Ƿ�ɹ�
	*/
	template<typename ReturnType, typename ClassType, typename... Args>
	bool PlayMontageAnim(UObject* PlayAnimAblyObj, MemberFunctionPtr<ReturnType, ClassType, Args...> MontageAimnNotifyDelaFunPtr, FString& AnimName, FString& NotifyName,float NotifyTime);
public:
	//��ȡ���ж�������,1Anim->1SkeletalMesh
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
