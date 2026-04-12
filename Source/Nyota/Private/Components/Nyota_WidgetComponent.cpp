// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Nyota_WidgetComponent.h"
#include "UI/Widget/HealthBar.h"
#include "AbilitySystemComponent.h"
#include "Characters/Enemy/EnemyCharacter.h"

void UNyota_WidgetComponent::InitWidget()
{
    Super::InitWidget();

    UHealthBar* HealthWidget = Cast<UHealthBar>(GetUserWidgetObject());
    if(HealthWidget)
    {
        
        HealthWidget->InitialHealthBar(Cast<AEnemyCharacter>(GetOwner())->GetAbilitySystemComponent());
    }
}