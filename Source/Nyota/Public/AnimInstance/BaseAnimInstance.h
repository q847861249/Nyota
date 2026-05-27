// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseAnimInstance.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class NYOTA_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	void SetupSpeed();
	
	void SetupDirection();
	
	UPROPERTY()
	ABaseCharacter* CurrentCharacter;
	UPROPERTY(BlueprintReadWrite)
	UCharacterMovementComponent* MovementComponent;
	UPROPERTY(BlueprintReadWrite)
	float MoveDirection;
	UPROPERTY(BlueprintReadWrite)
	FVector MovementVelocity;
	UPROPERTY(BlueprintReadWrite)
	float GroundSpeed;
};
