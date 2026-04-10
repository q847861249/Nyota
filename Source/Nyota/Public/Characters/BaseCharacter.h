// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnASCReady,UAbilitySystemComponent*,ASC);
UCLASS()
class NYOTA_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
private:
	UPROPERTY(Replicated)
	bool bAlive = true;

	// mounted initial abilities and effects
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<class UGameplayAbility>> StartingAbilities;

	UPROPERTY(EditDefaultsOnly,Category = "Effect")
	TSubclassOf<class UGameplayEffect> InitialGameplayEffect;
protected:
	// Init start abilities from StartingAbilities array
	void SetStartAbilities();
	// Init attribute set
	void InitializedAttributes();

public:	
	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Replicated function to setup replication for bAlive
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return nullptr;}

	FOnASCReady OnASCReady;	

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const {return bAlive;}

	UFUNCTION(BlueprintCallable)
	void SetAlive(bool NewAlive) {bAlive = NewAlive;}

};
