// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NyotaPlayer.h"
#include "Net/UnrealNetwork.h"

//Input
#include "Input/NyotaEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/NyotaGameplayTags.h"
#include "EnhancedInputComponent.h"

//Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

//Component
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
//Debug
#include "Debug/Debug.h"

ANyotaPlayer::ANyotaPlayer()
{
	//PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	GetCapsuleComponent()->bReplicatePhysicsToAutonomousProxy = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//create ArrowComponent For shooting;
	ShootingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootingArrow"));
	ShootingArrow->SetupAttachment(RootComponent);
}

void ANyotaPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerInputData->DefaultMappingContext, 0);
		}
	}
}

void ANyotaPlayer::BeginPlay()
{
	Super::BeginPlay();


	

}

void ANyotaPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

	}
}

void ANyotaPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
	
		AddControllerPitchInput(LookAxisVector.Y);
		
	}
}

void ANyotaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UNyotaEnhancedInputComponent* EnhancedInputComponent = Cast<UNyotaEnhancedInputComponent>(PlayerInputComponent)) {
		

		//Native InputAction;
		const FNyotaGameplayTags& GamplayTags = FNyotaGameplayTags::Get();

		EnhancedInputComponent->BindNativeActionWithTag(PlayerInputData, GamplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ANyotaPlayer::Move);
		EnhancedInputComponent->BindNativeActionWithTag(PlayerInputData, GamplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ANyotaPlayer::Look);
		EnhancedInputComponent->BindNativeActionWithTag(PlayerInputData, GamplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &ANyotaPlayer::Jump);

	}

}
