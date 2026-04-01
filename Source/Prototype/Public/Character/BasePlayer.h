// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "BasePlayer.generated.h"

/**
 *
 */
UCLASS()
class PROTOTYPE_API ABasePlayer : public ABaseCharacter
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

protected:
	void MoveInput(const FInputActionValue &Value);

	void LookInput(const FInputActionValue &Value);

	void Skill_1_Input(const FInputActionValue &Value);

	void Skill_2_Input(const FInputActionValue &Value);

	void Skill_3_Input(const FInputActionValue &Value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext *InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *Skill_1_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *Skill_2_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction *Skill_3_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Tag")
	FGameplayTagContainer Skill_1_Container;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Tag")
	FGameplayTagContainer Skill_2_Container;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Tag")
	FGameplayTagContainer Skill_3_Container;
};
