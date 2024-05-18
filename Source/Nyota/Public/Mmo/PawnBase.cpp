// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Animation/AnimMontage.h"  
#include "Animation/AnimInstance.h"
//#include "SkillSystem/EffectAbility.h"
DEFINE_LOG_CATEGORY(MmoFrameLog)

UAbilitySystemComponent* APawnBase::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �޸ģ�ʵ����ASC
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	
	
}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
	Super::BeginPlay();

	//���¼���������ʼ���ŵ�BeginPlay,�����ڼ�����ͼʱ�������
	this->EventTriggerObj = StaticCast<EventTrigger<FString>*>(FMemory::Malloc(sizeof(EventTrigger<FString>)));

	if (nullptr != AbilitySystem)
	{
		// �޸ģ���ASC���輼��
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

		// �޸ģ���ʼ��ASC
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	//�����¼�������
}

void APawnBase::OnTimerNotify(FString NotifyName)
{
	//TArray<FString> Parts;
	//NotifyName.ParseIntoArray(Parts, TEXT("_"));
	
	//auto* AnimNotifyDelagateInfo = this->FindFAnimNotifyDelagateInfo(NotifyName);
	//if (!AnimNotifyDelagateInfo)
	//{
	//	// ���û���ҵ���̫�棬�������������������  
	//	UE_LOG(LogTemp, Error, TEXT("APawnBase::PlayMontageAnim AnimNotifyDelagateInfo Have Not Found"));

	//}

	//AnimNotifyDelagateInfo->MontageAnimNotifyDelagate->Broadcast(*NotifyName);
	UE_LOG(LogTemp, Error, TEXT("APawnBase::PlayMontageAnim AnimNotifyDelagateInfo:NotifyName=%s,ObjName=%s"), *NotifyName, *this->GetName());
}

void APawnBase::OnStatusNotify(FString NotifyName)
{
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->EventTriggerObj)
	{
		this->EventTriggerObj->Tick(DeltaTime);
	}

}

// Called to bind functionality to input
void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

}

template<typename ReturnType, typename ClassType, typename... Args>
void APawnBase::BindMontageAnimNotify(UObject* BindObj, FString& AnimName, FAnimNotifyDelagateInfo<ReturnType, ClassType, Args...>* AnimNotifyDelagateInfo)
{
	FString MontAnimNotifyName = AnimName.Append(AnimNotifyDelagateInfo->NotifyName);
	auto AnimNotifyDelagateInfoArray = this->MontAnmNotifyDelMap.Find(MontAnimNotifyName);
	/*FAnimNotifyDelagateInfo* AnimNotifyDelagateInfo;*/
	//�����ж϶����Ƿ��Ѿ���֪ͨ����
	if (!AnimNotifyDelagateInfoArray)
	{
		//������Ӧ������֪ͨ������

		auto AnimNotifyDelagateInfo1 = StaticCast<FAnimNotifyDelagateInfo<void, UObject, FString&>*>(FMemory::Malloc(sizeof(FAnimNotifyDelagateInfo<void, UObject, FString&>)));
		AnimNotifyDelagateInfo1 = StaticCast<FAnimNotifyDelagateInfo<void, UObject, FString&>*>(AnimNotifyDelagateInfo);
		TArray<FAnimNotifyDelagateInfo<void, UObject, FString&>*>* AnimNotifyDelagateInfoArray1 = StaticCast<TArray<FAnimNotifyDelagateInfo<void, UObject, FString&>*>*>(FMemory::Malloc(sizeof(TArray<FAnimNotifyDelagateInfo<void, UObject, FString&>*>)));
		AnimNotifyDelagateInfoArray1->Add(AnimNotifyDelagateInfo1);	
	}
	else
	{
		//Ȼ��Ҫ�ж϶�����ͬһʱ���Ƿ�Ҫ֪ͨ����,����жಥ
		auto* AnimNotifyDelagateInfo1 = this->FindFAnimNotifyDelagateInfo(AnimNotifyDelagateInfo->NotifyName);
		//���������ظ����
		if (AnimNotifyDelagateInfo1)
		{
				//TMulticastDelegate<void(FString)>* NotifyDel = StaticCast<TMulticastDelegate<void(FString)>*>(FMemory::Malloc(sizeof(TMulticastDelegate<void(FString)>)));
				//BindObj->AnimNotifyDelagateInfo->MontageAnimNotifyFunPtr
				//NotifyDel->AddUObject(this, &APawnBase::OnTimerNotify);
			//struct ExtractFirstAndRest {
			//	using Type = typename FirstAndRest<Args...>::FirstType;
			//	using Rest = typename FirstAndRest<Args...>::RestTypes;
			//};
			//using Type = typename ExtractFirstAndRest::Type;
			MemberFunctionPtr<void, ClassType, FString> MontageAnimNotifyFunPtr = (MemberFunctionPtr<void, ClassType, FString>)AnimNotifyDelagateInfo->MontageAnimNotifyFunPtr;
			AnimNotifyDelagateInfo1->MontageAnimNotifyDelagate->AddUObject(
				//[&](FString& NotifyName) {
				//	//(*AnimNotifyDelagateInfo.MontageAnimNotifyFunPtr)(AnimNotifyDelagateInfo->NotifyName);
				//	//MontageAnimNotifyDelagateFunPtr MontageAnimNotifyFunPtr;
				//	((ClassType*)BindObj->*AnimNotifyDelagateInfo->MontageAnimNotifyFunPtr)(NotifyName);
				//}
					BindObj, MontageAnimNotifyFunPtr
			);
			return;
		}
		
	}
			
}

FAnimNotifyDelagateInfo<void, UObject, FString*>* APawnBase::FindFAnimNotifyDelagateInfo(FString& MontaqeAnimNotifyName)
{
	
	// ���� MontAnimNotifyDelMap ��һ�� TMap<FString, TArray<FAnimNotifyDelagateInfo<...>*>*>  
	TArray<FString> Parts;
	MontaqeAnimNotifyName.ParseIntoArray(Parts, TEXT("_"), false);
	auto* AnimNotifyDelagateInfoArrayPtr = this->MontAnmNotifyDelMap.Find(Parts[0]);

	if (AnimNotifyDelagateInfoArrayPtr) // ȷ���ҵ��˶�Ӧ������  
	{
		auto AnimNotifyDelagateInfoArray = *AnimNotifyDelagateInfoArrayPtr;
		for (auto& AnimNotifyDelagateInfo : AnimNotifyDelagateInfoArray)
		{
			if (AnimNotifyDelagateInfo->NotifyName == MontaqeAnimNotifyName)
			{
				return &(*AnimNotifyDelagateInfo);
			}
		}
	}
	return nullptr;
}

void APawnBase::OnAnimNotifyOneParam(FString& NotifyName)
{
	UE_LOG(LogTemp, Log, TEXT("NotifyName:%s"), *NotifyName);
}

template<typename ReturnType, typename ClassType, typename... Args>
void APawnBase::PlayMontageAnim(USkeletalMeshComponent* AnimSkeletal, FString& AnimName, FString& NotifyName)
{
	// ��ȡAnimInstance�����Ƕ������ŵ�ʵ��  
	UAnimInstance* AnimInstance = AnimSkeletal->GetAnimInstance();
	
	// ������AnimInstance�в�����̫��  
	UAnimSequence* Montage = LoadObject<UAnimSequence>(AnimSkeletal, *AnimName);
	//AnimInstance->Montage_Play(Montage)
	// ������̫����Դ  
	
	//UAnimMontage AnimMontage;
	//AnimMontage.LoadConfig(UAnimMontage::StaticClass(), *AnimName);
	if (Montage)
	{
		// ������̫��  
		AnimInstance->PlaySlotAnimationAsDynamicMontage_WithBlendArgs(Montage, FName("Attack"),
			0,0 );
		/*EventTrigger<FString>* EventTrigger1 = this->EventTriggerObj;*/

		FString& MontaqeAnimNotifyName = AnimName.Append(NotifyName);
		auto* AnimNotifyDelagateInfo = this->FindFAnimNotifyDelagateInfo(MontaqeAnimNotifyName);
		if (!AnimNotifyDelagateInfo)
		{
			// ���û���ҵ���̫�棬�������������������  
			UE_LOG(LogTemp, Error, TEXT("APawnBase::PlayMontageAnim AnimNotifyDelagateInfo Have Not Found"));
			return;
		}
		////@Note �������Ҫ�ͷŵ�Map��ӦAnimNotify��TArray,������ø��ݶ�����֡�ʽ���ʱ�����

		//auto b = MakeShared<TFunction<void()>>(
		//	// Lambda���ʽ���������AnimNotifyDelagateInfo������  
		//	[&]()
		//	{
		//		// ����MontageAnimNotifyDelagate��Broadcast������������NotifyName  
		//		AnimNotifyDelagateInfo->MontageAnimNotifyDelagate->Broadcast(NotifyName);
		//	}
		//);

		//TDelegate<void(FString)>* NotifyDel = StaticCast<TDelegate<void(FString)>*>(FMemory::Malloc(sizeof(TDelegate<void(FString*)>)));

		//NotifyDel->BindUObject(this, &APawnBase::OnTimerNotify);

		//�ಥί�У���ʱ��󴥷����а󶨵�֪ͨ
		
		this->EventTriggerObj->AddTimerTrigger(
			(TMulticastDelegate<void(FString*)>*)AnimNotifyDelagateInfo->MontageAnimNotifyDelagate,
			this->GetWorld(),
			AnimNotifyDelagateInfo->NotifyTime
		);
		//AnimInstance->Montage_Play(&AnimMontage, 1.0f, EMontagePlayReturnType::MontageLength);
	}
	else
	{
		// ���û���ҵ���̫�棬�������������������  
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

	auto* AnimNotifyDelagateInfo = StaticCast<FAnimNotifyDelagateInfo<void, UObject, FString&>*>(FMemory::Malloc(sizeof(FAnimNotifyDelagateInfo<void, UObject, FString&>)));
	AnimNotifyDelagateInfo->NotifyName = NotifyName;
	AnimNotifyDelagateInfo->NotifyTime = NotifyTime;
	
	AnimNotifyDelagateInfo->MontageAnimNotifyFunPtr = StaticCast<MemberFunctionPtr<void, UObject, FString&>>(MontageAimnNotifyDelaFunPtr);
	FMontageAnimNotifyDelagate * MontageAnimNotifyDelagate = StaticCast<FMontageAnimNotifyDelagate*>(FMemory::Malloc(sizeof(FMontageAnimNotifyDelagate)));
	AnimNotifyDelagateInfo->MontageAnimNotifyDelagate = MontageAnimNotifyDelagate;

	//PlayAnimAblyObj�󶨶���֪ͨ
	this->BindMontageAnimNotify<void, UObject, FString&>(PlayAnimAblyObj, AnimName, AnimNotifyDelagateInfo);
	this->PlayMontageAnim<ReturnType, ClassType, Args...>(SkeletalMeshComponent1, AnimName, NotifyName);

	return true;
}

void APawnBase::BeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString AnimName = FString("/Script/Engine.AnimMontage'/Game/Demo/CharacterAsset/Character_Attack_Montage.Character_Attack_Montage'");
	FString NotifyName = FString("USkillAbility001");
	float NotifyTime = 1.f;
	this->PlayMontageAnim<void, APawnBase, FString&>(this, &APawnBase::OnAnimNotifyOneParam, AnimName, NotifyName, NotifyTime);

}
