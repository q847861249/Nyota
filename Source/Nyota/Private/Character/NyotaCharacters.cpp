// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NyotaCharacters.h"

// Sets default values
ANyotaCharacters::ANyotaCharacters()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
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

}

void ANyotaCharacters::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystem->InitAbilityActorInfo(this,this);

	GiveAbility();

}

// Called every frame
void ANyotaCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANyotaCharacters::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

