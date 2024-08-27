// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "NyotaMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API UNyotaMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	class FSavedMove_M : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

	public:
		enum CompressedFlags
		{
			FLAG_CanCombo = 0x10,
			FLAG_CanAttack = 0x20,
			FLAG_ComboIndex = 0x40,

		};

	public:

		// Flags
		uint8 Saved_bCanAttack : 1;
		uint8 Saved_bCanCombo : 1;
		uint8 Saved_ComboIndex : 1;


		FSavedMove_M();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_M : public FNetworkPredictionData_Client_Character
	{

	public:

		FNetworkPredictionData_Client_M(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:

	UPROPERTY(BlueprintReadWrite)

	bool Safe_bInAttack = false;


	UPROPERTY(BlueprintReadWrite)
	bool Safe_bComboWindow = false;

	UPROPERTY(BlueprintReadWrite)
	bool Safe_ComboIndex = 0;


};
