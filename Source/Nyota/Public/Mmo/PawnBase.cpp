// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Animation/AnimMontage.h"  
#include "Animation/AnimInstance.h"
//#include "SkillSystem/EffectAbility.h"

UAbilitySystemComponent* APawnBase::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 修改：实例化ASC
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	
	this->EventTrigger = NewObject<UEventTrigger>();
}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (nullptr != AbilitySystem)
	{
		// 修改：给ASC赋予技能
		if (HasAuthority() && MyAbilities.Num() > 0)
		{
			for (auto i = 0; i < MyAbilities.Num(); i++)
			{
				if (MyAbilities[i] == nullptr)
				{
					continue;
				}
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(MyAbilities[i].GetDefaultObject(), 1, 0));
			}
		}

		// 修改：初始化ASC
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	//创建事件触发器
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TArray<USkeletalMeshComponent> SkeletalMeshComponentArray;
	//GetComponents<USkeletalMeshComponent>(SkeletalMeshComponentArray);

	//动画事件监听
	//USkeletalMeshComponent* skeletalMeshComp = this->GetSkeletalMeshComponent();
	//for (int i=0; i< SkeletalMeshComponentArray.Num(); i++)
	//{
	//	USkeletalMeshComponent&  SkeletalMeshComponent = SkeletalMeshComponentArray[i];
	//	UAnimInstance* AnimInstance = SkeletalMeshComponent.GetAnimInstance();
	//	if (AnimInstance)
	//	{
	//		AnimInstance->PlaySlotAnimationAsDynamicMontage
	//		//AnimInstance->PlaySlotAnimationAsDynamicMontage_WithBlendArgs
	//		SkeletalMeshComponent.AnimationData
	//		AnimInstance->GetCurrentAnim()
	//		// 假设你知道要检查的动画名称或索引  
	//		FAnimSequencePlayer::FAnimationData animationData = AnimInstance->GetCurrentAnimationData();
	//		if (animationData.IsValid() && animationData.Animation == MyAnimationSequence)
	//		{
	//			// 检查动画播放进度或其他状态  
	//			float playRate = animInstance->GetPlayRate();
	//			float currentPosition = AnimInstance->GetCurrentPosition();
	//			// ... 执行你的逻辑 ...  
	//		}
	//	}
	//}

}

// Called to bind functionality to input
void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//D:\Program Files(x86)\UE_5.4\Engine\Source\Runtime\Core\Public\Delegates\DelegateInstanceInterface.h

template<typename ReturnType, typename ClassType, typename... Args>
void APawnBase::BindMontageAnimNotify(UObject* BindObj, FString& AnimName, FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* AnimNotifyDelagateInfo)
{
	auto AnimNotifyDelagateInfoArray = this->MontAnmNotifyDelMap.Find(AnimName);
	/*FAnimNotifyDelagateInfo* AnimNotifyDelagateInfo;*/
	//首先判断动画是否已经有通知数组
	if (!AnimNotifyDelagateInfoArray)
	{
		//创建对应动画的通知绑定数组

		auto AnimNotifyDelagateInfo1 = StaticCast<FAnimNotifyDelagateInfo<void, UObject, FString&>*>(FMemory::Malloc(sizeof(FAnimNotifyDelagateInfo<void, UObject, FString&>)));
		AnimNotifyDelagateInfo1 = StaticCast<FAnimNotifyDelagateInfo<void, UObject, FString&>*>(AnimNotifyDelagateInfo);
		TArray<FAnimNotifyDelagateInfo<void, UObject, FString&>*>* AnimNotifyDelagateInfoArray1 = StaticCast<TArray<FAnimNotifyDelagateInfo<void, UObject, FString&>*>*>(FMemory::Malloc(sizeof(TArray<FAnimNotifyDelagateInfo<void, UObject, FString&>*>)));
		AnimNotifyDelagateInfoArray1->Add(AnimNotifyDelagateInfo1);

		//AnimNotifyDelagateInfoArray->Add(AnimNotifyDelagateInfo);
		this->MontAnmNotifyDelMap.Add(AnimName, *AnimNotifyDelagateInfoArray1);
		
	}
	else
	{
		//然后还要判断动画，同一时间是否还要通知数组,如果有多播
		FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* AnimNotifyDelagateInfo1 = this->FindFAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>(AnimName, AnimNotifyDelagateInfo->NotifyName);
		//存在无需重复添加
		if (AnimNotifyDelagateInfo1)
		{
			//FMontageAnimNotifyDelagate MontageAnimNotifyDelagate12;
			//MontageAnimNotifyDelagate12.AddRaw(this, &APawnBase::OnAnimNotifyOneParam);
			//MontageAnimNotifyDelagateFunPtr sd = &APawnBase::OnAnimNotifyOneParam;TMemFunPtrType<false, void, FString&>
			
			AnimNotifyDelagateInfo1->MontageAnimNotifyDelagate->AddLambda(
				[&]() {
					//(*AnimNotifyDelagateInfo.MontageAnimNotifyFunPtr)(AnimNotifyDelagateInfo->NotifyName);
					//MontageAnimNotifyDelagateFunPtr MontageAnimNotifyFunPtr;
					((ClassType*)BindObj->*AnimNotifyDelagateInfo->MontageAnimNotifyFunPtr)(AnimNotifyDelagateInfo->NotifyName);
				}
			);

			return;
		}
		
	}
			
}

//template<typename ReturnType, typename ClassType, typename... Args>
//FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* APawnBase::FindFAnimNotifyDelagateInfo(FString& AnimName, FString& NotifyName)
//{
//	TArray<FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>*>* AnimNotifyDelagateInfoArray = this->MontAnmNotifyDelMap.Find(AnimName);
//
//	for (TArray<FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>*>::TIterator AnimNotifyDelagateInfoIterator(*AnimNotifyDelagateInfoArray); AnimNotifyDelagateInfoIterator; ++AnimNotifyDelagateInfoIterator)
//	{
//		if ((*AnimNotifyDelagateInfoIterator)->NotifyName == NotifyName)
//		{
//			return *AnimNotifyDelagateInfoIterator;
//		}
//	}
//	return nullptr;
//}

template<typename ReturnType, typename ClassType, typename... Args>
FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* APawnBase::FindFAnimNotifyDelagateInfo(FString& AnimName, FString& NotifyName)
{
	// 假设 MontAnimNotifyDelMap 是一个 TMap<FString, TArray<FAnimNotifyDelagateInfo<...>*>*>  
	auto* AnimNotifyDelagateInfoArrayPtr = this->MontAnmNotifyDelMap.Find(AnimName);

	if (AnimNotifyDelagateInfoArrayPtr) // 确保找到了对应的数组  
	{
		auto AnimNotifyDelagateInfoArray = *AnimNotifyDelagateInfoArrayPtr;
		for (auto& AnimNotifyDelagateInfo : AnimNotifyDelagateInfoArray)
		{
			if (AnimNotifyDelagateInfo->NotifyName == NotifyName)
			{
				return /*(FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>*)*/AnimNotifyDelagateInfo;
			}
		}
	}
	return nullptr;
}

void APawnBase::OnAnimNotifyOneParam(FString& NotifyName)
{
	//TArray<FAnimNotifyDelagateInfo*>*AnimNotifyDelagateInfoArray = this->MontAnmNotifyDelMap.Find(AnimName);
	//for (TArray<FAnimNotifyDelagateInfo*>::TIterator AnimNotifyDelagateInfoIterator(*AnimNotifyDelagateInfoArray); AnimNotifyDelagateInfoIterator; ++AnimNotifyDelagateInfoIterator)
	//{
	//	if ((*AnimNotifyDelagateInfoIterator)->NotifyName == NotifyName)
	//	{
	//		(*AnimNotifyDelagateInfoIterator)->MontageAnimNotifyFunPtr(NotifyName);
	//	}
	//}
}

template<typename ReturnType, typename ClassType, typename... Args>
void APawnBase::PlayMontageAnim(USkeletalMeshComponent* AnimSkeletal, FString& AnimName, FString& NotifyName)
{
	// 获取AnimInstance，这是动画播放的实例  
	UAnimInstance* AnimInstance = AnimSkeletal->GetAnimInstance();
	
	// 尝试在AnimInstance中查找蒙太奇  
	UAnimSequence* Montage = LoadObject<UAnimSequence>(AnimSkeletal, *AnimName);
	//AnimInstance->Montage_Play(Montage)
	// 加载蒙太奇资源  
	
	//UAnimMontage AnimMontage;
	//AnimMontage.LoadConfig(UAnimMontage::StaticClass(), *AnimName);
	if (Montage)
	{
		// 播放蒙太奇  
		AnimInstance->PlaySlotAnimationAsDynamicMontage_WithBlendArgs(Montage, FName("DefaultSlot"),
			0,0 );
		UEventTrigger* EventTrigger1 = this->EventTrigger;

		FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* AnimNotifyDelagateInfo = FindFAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>(AnimName, NotifyName);
		if (!AnimNotifyDelagateInfo)
		{
			// 如果没有找到蒙太奇，输出错误或进行其他处理  
			UE_LOG(LogTemp, Error, TEXT("APawnBase::PlayMontageAnim AnimNotifyDelagateInfo Have Not Found"));

		}
		//@Note 这里可能要释放掉Map对应AnimNotify的TArray,这里因该根据动画的帧率进行时间计算
		EventTrigger1->AddTimerTrigger(
			[&]() {
				AnimNotifyDelagateInfo->MontageAnimNotifyDelagate->Broadcast(NotifyName);
			},
			AnimNotifyDelagateInfo->NotifyTime
		);
		//AnimInstance->Montage_Play(&AnimMontage, 1.0f, EMontagePlayReturnType::MontageLength);
	}
	else
	{
		// 如果没有找到蒙太奇，输出错误或进行其他处理  
		UE_LOG(LogTemp, Error, TEXT("APawnBase::PlayMontageAnim not found on SkeletalMeshComponent."));
	}

}

template<typename ReturnType, typename ClassType, typename ...Args>
bool APawnBase::PlayMontageAnim(UObject* PlayAnimAblyObj, MemberFunctionPtr<ReturnType, ClassType, Args...> MontageAimnNotifyDelaFunPtr, FString& AnimName, FString& NotifyName, float NotifyTime)
{
	//TArray<USkeletalMeshComponent> SkeletalMeshComponentArray;
	//GetComponents<USkeletalMeshComponent>(SkeletalMeshComponentArray);

	//for (int i=0; i< SkeletalMeshComponentArray.Num(); i++)
	//{

	//}
	USkeletalMeshComponent* SkeletalMeshComponent1 = *this->AnimSkeletalMeshMap.Find(AnimName);
	if (!SkeletalMeshComponent1)
	{
		UE_LOG(LogTemp, Error, TEXT("APawnBase::PlayMontageAnim  AnimName= %s Have Not Bind USkeletalMeshComponent"), *AnimName);
		return false;
	}

	FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* AnimNotifyDelagateInfo = StaticCast<FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>*>(FMemory::Malloc(sizeof(FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>)));
	AnimNotifyDelagateInfo->NotifyName = NotifyName;
	AnimNotifyDelagateInfo->NotifyTime = NotifyTime;
	AnimNotifyDelagateInfo->MontageAnimNotifyFunPtr = MontageAimnNotifyDelaFunPtr;
	FMontageAnimNotifyDelagate* MontageAnimNotifyDelagate = StaticCast<FMontageAnimNotifyDelagate*>(FMemory::Malloc(sizeof(FMontageAnimNotifyDelagate)));
	AnimNotifyDelagateInfo->MontageAnimNotifyDelagate = MontageAnimNotifyDelagate;

	//PlayAnimAblyObj绑定动画通知
	this->BindMontageAnimNotify<ReturnType, ClassType, Args...>(PlayAnimAblyObj, AnimName, AnimNotifyDelagateInfo);
	this->PlayMontageAnim<ReturnType, ClassType, Args...>(SkeletalMeshComponent1, AnimName, NotifyName);

	return true;
}

void APawnBase::BeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString AnimName = FString("asd");
	FString NotifyName = FString("USkillAbility001");
	float NotifyTime = 1.f;
	this->PlayMontageAnim<void, APawnBase, FString&>(this, &APawnBase::OnAnimNotifyOneParam, AnimName, NotifyName, NotifyTime);


}

