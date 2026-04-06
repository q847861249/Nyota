// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called to bind functionality to input
// void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);

// }
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bAlive);
}

void ABaseCharacter::SetStartAbilities()
{
	if(!GetAbilitySystemComponent()) return;
	if(StartingAbilities.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Starting Abilities set on %s"), *GetName());
		return;
	}
	//Register ability from StartingAbiities array by using ASC API(GiveAbility)
	for(const auto& Ability: StartingAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}

void ABaseCharacter::InitializedAttributes()
{
	if(!InitialGameplayEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitialGameplayEffect is not set on %s"), *GetName());
		return;
	}
	if(!GetAbilitySystemComponent()) return;
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitialGameplayEffect,1.0f,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}


