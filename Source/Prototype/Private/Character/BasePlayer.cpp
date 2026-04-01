// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BasePlayer.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void ABasePlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController *PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem *LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!LocalPlayerSubsystem)
	{
		return;
	}
	if (InputMappingContext)
	{
		LocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: InputMappingContext Is Null."), *GetNameSafe(this));
	}

	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::LookInput);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: LookAction Is Null."), *GetNameSafe(this));
		}

		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::MoveInput);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: MoveAction Is Null."), *GetNameSafe(this));
		}

		if (Skill_1_Action)
		{
			EnhancedInputComponent->BindAction(Skill_1_Action, ETriggerEvent::Triggered, this, &ABasePlayer::Skill_1_Input);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Skill_1_Action Is Null."), *GetNameSafe(this));
		}

		if (Skill_2_Action)
		{
			EnhancedInputComponent->BindAction(Skill_2_Action, ETriggerEvent::Triggered, this, &ABasePlayer::Skill_2_Input);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Skill_2_Action Is Null."), *GetNameSafe(this));
		}

		if (Skill_3_Action)
		{
			EnhancedInputComponent->BindAction(Skill_3_Action, ETriggerEvent::Triggered, this, &ABasePlayer::Skill_3_Input);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Skill_3_Action Is Null."), *GetNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: EnhancedInputComponent Is Null."), *GetNameSafe(this));
	}
}

void ABasePlayer::MoveInput(const FInputActionValue &Value)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D MoveVector2D = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVector2D.Y);
	AddMovementInput(RightDirection, MoveVector2D.X);
}

void ABasePlayer::LookInput(const FInputActionValue &Value)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D LookVector2D = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector2D.X);
	AddControllerPitchInput(LookVector2D.Y);
}

void ABasePlayer::Skill_1_Input(const FInputActionValue &Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill_1_Input Function Call"));
}

void ABasePlayer::Skill_2_Input(const FInputActionValue &Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill_2_Input Function Call"));
}

void ABasePlayer::Skill_3_Input(const FInputActionValue &Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill_3_Input Function Call"));
}
