// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BaseAnimInstance.h"

#include "Character/BaseCharacter.h"
#include "KismetAnimationLibrary.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	MoveDirection = 0.0f;
	MovementVelocity = FVector::ZeroVector;
	GroundSpeed = 0.0f;

	APawn* Owner = TryGetPawnOwner();
	if (!Owner)
	{
		return;
	}

	CurrentCharacter = Cast<ABaseCharacter>(Owner);
	if (!CurrentCharacter)
	{
		return;
	}

	MovementComponent = CurrentCharacter->GetCharacterMovement();
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!CurrentCharacter || !MovementComponent)
	{
		GroundSpeed = 0.0f;
		
		return;
	}
	
	SetupSpeed();
	
	SetupDirection();
}

void UBaseAnimInstance::SetupSpeed()
{
	if (!MovementComponent)
	{
		return;
	}
	
	MovementVelocity = MovementComponent->Velocity;
	
	// 走路/奔跑通常只看水平速度（X/Y），Z 只用于飞行/跳跃/落下类动画，这里目前只处理走路，所以用 Size2D 而不是 Length
	GroundSpeed = MovementVelocity.Size2D();
}

void UBaseAnimInstance::SetupDirection()
{
	if (!CurrentCharacter)
	{
		return;
	}
	
	const FRotator ActorRotation = CurrentCharacter->GetActorRotation();
	MoveDirection = UKismetAnimationLibrary::CalculateDirection(MovementVelocity, ActorRotation);
}
