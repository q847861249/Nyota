// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Nyota_PlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FGameplayTag;
UCLASS()
class NYOTA_API ANyota_PlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void SetupInputComponent() override;
	void ActivateAbilityByTag(const FGameplayTag& AbilityTag);
public:
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem;
	// input mapping context
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputContext")
	TObjectPtr<UInputMappingContext> MovingMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputContext")
	TObjectPtr<UInputMappingContext> AbilitiesMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputContext")
	TObjectPtr<UInputMappingContext> GameplayMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputContext")
	TObjectPtr<UInputMappingContext> UIMappingContext;

	//input actions
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputAction")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputAction")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputAction")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputAction")
	UInputAction* PrimaryAction;
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputAction")
	UInputAction* QAction;
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputAction")
	UInputAction* EAction;
	UPROPERTY(EditDefaultsOnly, Category = "Nyota_|InputAction")
	UInputAction* RAction;


	// actions callback functions
	void Move(const FInputActionValue& Value);
	void StopMove();
	void StartJump();
	void StopJump();
	void Look(const FInputActionValue& Value);
	void Primary();
	void QAbility();
	void EAbility();
	void RAbility();
};
