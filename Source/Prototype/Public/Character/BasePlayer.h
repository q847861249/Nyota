// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Enums/PlayerType.h"

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

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    virtual void PossessedBy(AController *NewController) override;

    virtual void OnRep_PlayerState() override;

    virtual UAttributeSet *GetAttributeSet() const override;

    virtual void Tick(float DeltaSeconds) override;

protected:
    void MoveInput(const FInputActionValue &Value);

    void LookInput(const FInputActionValue &Value);

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void WaterBallAttack(const FGameplayTag &ActivateTag);
    
    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void GrabSlamAttack(const FGameplayTag &ActivateTag);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputMappingContext *InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputAction *LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputAction *MoveAction;

    float TargetYaw; // 记录目标朝向

private:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float RotationInterpSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_Forward = 90.f; // 纯前后

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_Side = 0.f; // 纯左右

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_ForwardLeft = 45.f; // W+A

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_ForwardRight = -45.f; // W+D

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_BackLeft = -45.f; // S+A

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Movement | Rotation")
    float YawOffset_BackRight = 45.f; // S+D
};
