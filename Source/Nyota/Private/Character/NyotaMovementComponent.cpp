// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NyotaMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

UNyotaMovementComponent::FSavedMove_M::FSavedMove_M()
{
	Saved_bCanAttack = 0;
	Saved_bCanCombo = 0;
	Saved_ComboIndex = 0;
}

bool UNyotaMovementComponent::FSavedMove_M::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_M* New_M_Move = static_cast<FSavedMove_M*>(NewMove.Get());

	if (Saved_bCanAttack != New_M_Move->Saved_bCanAttack) {
		return false;
	}
	if (Saved_bCanCombo != New_M_Move->Saved_bCanCombo) {
		return false;
	}
	if (Saved_ComboIndex != New_M_Move->Saved_ComboIndex) {
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UNyotaMovementComponent::FSavedMove_M::Clear()
{
	Saved_bCanAttack = 0;
	Saved_bCanCombo = 0;
	Saved_ComboIndex = 0;
}

uint8 UNyotaMovementComponent::FSavedMove_M::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (Saved_bCanCombo) Result |= FLAG_CanCombo;
	if (Saved_bCanAttack) Result |= FLAG_CanAttack;
	if (Saved_ComboIndex) Result |= FLAG_ComboIndex;

	return Result;
}

void UNyotaMovementComponent::FSavedMove_M::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const UNyotaMovementComponent* CharacterMovement = Cast<UNyotaMovementComponent>(C->GetCharacterMovement());

	Saved_bCanAttack = CharacterMovement->Safe_bInAttack;

	Saved_bCanCombo = CharacterMovement->Safe_bComboWindow;

	Saved_ComboIndex = CharacterMovement->Safe_ComboIndex;

}

void UNyotaMovementComponent::FSavedMove_M::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UNyotaMovementComponent* CharacterMovement = Cast<UNyotaMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bInAttack = Saved_bCanAttack;

	CharacterMovement->Safe_bComboWindow = Saved_bCanCombo;

	CharacterMovement->Safe_ComboIndex = Saved_ComboIndex;
}

UNyotaMovementComponent::FNetworkPredictionData_Client_M::FNetworkPredictionData_Client_M(const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{

}

FSavedMovePtr UNyotaMovementComponent::FNetworkPredictionData_Client_M::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_M());
}
