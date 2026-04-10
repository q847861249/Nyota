// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/Nyota_AttributeSet.h"
#include "Net/UnrealNetwork.h"

void UNyota_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UNyota_AttributeSet, bBaseAttributesInitialized, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNyota_AttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNyota_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNyota_AttributeSet, Attack, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNyota_AttributeSet, Defense, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNyota_AttributeSet, MaxScore, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UNyota_AttributeSet, Score, COND_None, REPNOTIFY_Always);
}
void UNyota_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);
    if(!bBaseAttributesInitialized)
    {
        bBaseAttributesInitialized = true;
        OnBaseAttributeInitialized.Broadcast();
    }
    
    if(GetHealth()<0.0f)
    {
        Health = 0;
    }
    if(GetScore()>GetMaxScore())
    {
        SetScore(GetMaxScore());
    }
}

void UNyota_AttributeSet::OnRep_bBaseAttributesInitialized()
{
    // Notify clients that base attributes have been initialized
    if(bBaseAttributesInitialized)
    {
        OnBaseAttributeInitialized.Broadcast();
    }
}

void UNyota_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    //for the FGameplayAttributeData property, we need to use the provided macro to handle replication notification.
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNyota_AttributeSet, Health, OldHealth);
}

void UNyota_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNyota_AttributeSet, MaxHealth, OldMaxHealth);
}


void UNyota_AttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNyota_AttributeSet, Attack, OldAttack);
}


void UNyota_AttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNyota_AttributeSet, Defense, OldDefense);
}


void UNyota_AttributeSet::OnRep_MaxScore(const FGameplayAttributeData& OldMaxScore)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNyota_AttributeSet, MaxScore, OldMaxScore);
}


void UNyota_AttributeSet::OnRep_Score(const FGameplayAttributeData& OldScore)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UNyota_AttributeSet, Score, OldScore);
}