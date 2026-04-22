// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Nyota_AttributeSet.generated.h"

//These macros can reduce the need to write some Get and Set functions.
#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
//delegate for broadcasting when base atrribute is initialized
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBaseAttributeInitialized);
UCLASS()
class NYOTA_API UNyota_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	// automotically replicate properties
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// called when a GE modified base attribute value.
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// Base attributes value
	UPROPERTY(BlueprintAssignable)
	FBaseAttributeInitialized OnBaseAttributeInitialized;
	/**Check if the attribute already init */
	UPROPERTY(ReplicatedUsing = OnRep_bBaseAttributesInitialized)
	bool bBaseAttributesInitialized = false;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_AttackRange)
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxScore)
	FGameplayAttributeData MaxScore;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Score)
	FGameplayAttributeData Score;



	// Callback functions
	UFUNCTION()
	void OnRep_bBaseAttributesInitialized();

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack);

	UFUNCTION()
	void OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange);

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense);

	UFUNCTION()
	void OnRep_MaxScore(const FGameplayAttributeData& OldMaxScore);

	UFUNCTION()
	void OnRep_Score(const FGameplayAttributeData& OldScore);

	// enable macro to generate Get and Set functions for attributes
	ATTRIBUTE_ACCESSORS(UNyota_AttributeSet,Health);
	ATTRIBUTE_ACCESSORS(UNyota_AttributeSet,MaxHealth);
	ATTRIBUTE_ACCESSORS(UNyota_AttributeSet,Attack);
	ATTRIBUTE_ACCESSORS(UNyota_AttributeSet,AttackRange);
	ATTRIBUTE_ACCESSORS(UNyota_AttributeSet,Defense);
	ATTRIBUTE_ACCESSORS(UNyota_AttributeSet,MaxScore);
	ATTRIBUTE_ACCESSORS(UNyota_AttributeSet,Score);
};
