// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NyotaCharacters.h"

#include "Net/UnrealNetwork.h"
#include "Input/NyotaEnhancedInputComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Component/NyotaComponent.h"
#include "Input/NyotaGameplayTags.h"
#include "Ability/NyotaAttributeSet.h"


#include "Debug/Debug.h"

// Sets default values
ANyotaCharacters::ANyotaCharacters()
{
 	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	//Actor同步
	bReplicates = true;

	//角色基础信息
	NyotaComponent = CreateDefaultSubobject<UNyotaComponent>(TEXT("NyotaCharacterComponent"));

	// GAS basic setting
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//GAS attribute
	AttributeSet = CreateDefaultSubobject<UNyotaAttributeSet>(TEXT("AttributeSet"));

}

// Called when the game starts or when spawned
void ANyotaCharacters::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* ANyotaCharacters::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ANyotaCharacters::GiveAbility()
{
	if (nullptr != AbilitySystem)
	{
		// 修改：给ASC赋予技能
		if (HasAuthority() && NyotaComponent->CharacterConfig->CharacterAbilityConfig->NyotaAbilities.Num() > 0)
		{
			for (auto i = 0; i < NyotaComponent->CharacterConfig->CharacterAbilityConfig->NyotaAbilities.Num(); i++)
			{
				if (NyotaComponent->CharacterConfig->CharacterAbilityConfig->NyotaAbilities[i] == nullptr)
				{
					continue;
				}
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(NyotaComponent->CharacterConfig->CharacterAbilityConfig->NyotaAbilities[i].GetDefaultObject(), 1, 0));
			}
		}
		// 修改：初始化ASC
		//AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

void ANyotaCharacters::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystem->InitAbilityActorInfo(this,this);

	GiveAbility();
	ApplyStartUpEffect();
}

bool ANyotaCharacters::ApplyGameplayEffectToself(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle inEffectHandle)
{
	if (!Effect.Get()) return false;

	FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(Effect, 1, inEffectHandle);

	if (SpecHandle.IsValid()) {

		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGEHandle.WasSuccessfullyApplied();

	}

	return false;
}

void ANyotaCharacters::ApplyStartUpEffect()
{
	if (GetLocalRole() == ROLE_Authority) {

		FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();

		EffectContext.AddSourceObject(this);

		for (auto& CharacterEffect : NyotaComponent->CharacterConfig->CharacterAbilityConfig->DefaultCharacterInfomation) {

			ApplyGameplayEffectToself(CharacterEffect, EffectContext);

		}
	}
}

bool ANyotaCharacters::TryActiveAbilityByTag(FGameplayTag Tag)
{
	FGameplayTagContainer contatiner;

	contatiner.AddTag(Tag);

	bool ActiveResult = AbilitySystem->TryActivateAbilitiesByTag(contatiner);

	if (ActiveResult) return true;

	else {	
		return false;
	}
	
}

void ANyotaCharacters::EnableRagDoll_Implementation()
{
	Rep_EanbleRagdoll_Multicast();
}


void ANyotaCharacters::Rep_EanbleRagdoll_Multicast_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);

	GetCapsuleComponent()->SetCollisionProfileName("CharacterDead");

	GetMesh()->SetCollisionProfileName("Ragdoll");

	if (APlayerController* PlayerController = Cast<APlayerController> (GetController())) {
		DisableInput(PlayerController);
	}

}

// Called every frame
void ANyotaCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



