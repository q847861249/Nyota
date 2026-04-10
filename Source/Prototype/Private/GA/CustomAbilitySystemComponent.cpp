// Fill out your copyright notice in the Description page of Project Settings.

#include "GA/CustomAbilitySystemComponent.h"

UCustomAbilitySystemComponent::UCustomAbilitySystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCustomAbilitySystemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCustomAbilitySystemComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
