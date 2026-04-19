// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/Nyota_AbilitySystemComponent.h"
#include "AttributeSet/Nyota_AttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
    AbilitySystemComponent = CreateDefaultSubobject<UNyota_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    // Set Mixed replication mode so that only the necessary data (GE info) is replicated to clients, reducing bandwidth usage.
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    BaseAttributeSet = CreateDefaultSubobject<UNyota_AttributeSet>(TEXT("BaseAttributeSet"));
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    if(!IsValid(GetAbilitySystemComponent())) return;

    GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
    // if(!HasAuthority()) return;
    SetStartAbilities();
    InitializedAttributes();

}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    if(!IsValid(GetAbilitySystemComponent())) return;

    GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
    if(!HasAuthority()) return;
    SetStartAbilities();
    InitializedAttributes();

    UNyota_AttributeSet* AttributeSet = Cast<UNyota_AttributeSet>(GetAttributeSet());
    if(!IsValid(AttributeSet))
    {
        UE_LOG(LogTemp,Warning,TEXT("没有属性集"))
        return;  
    } 
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAttributeSet* AEnemyCharacter::GetAttributeSet()
{
    return BaseAttributeSet;
}