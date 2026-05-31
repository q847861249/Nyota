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
class NYOTA_API ABasePlayer : public ABaseCharacter
{
    GENERATED_BODY()

public:
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    virtual void PossessedBy(AController *NewController) override;

    virtual void OnRep_PlayerState() override;

    virtual UAttributeSet *GetAttributeSet() const override;

    virtual void Tick(float DeltaSeconds) override;

    virtual void BeginPlay() override;

protected:
    void MoveInput(const FInputActionValue &Value);

    void LookInput(const FInputActionValue &Value);

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void WaterBallAttack();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void GrabSlamAttack();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void WaterBubbleAttack();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void WaterBubbleAttackEnd();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void VortexGripAttack();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void GrabAttack();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnLightAttack_Started();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnLeftLightAttack_Started();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnRightLightAttack_Started();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnSkill_1_Started();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnSkill_1_Completed();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnSkill_2_Started();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnSkill_2_Completed();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnSkill_3_Started();

    UFUNCTION(BlueprintCallable, Category = "Nyota")
    void OnSkill_3_Completed();

    /**玩家角色的捕获体触发碰撞后执行 */
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult
    );

    /**
     * @brief 抓取前后激活 GA 的逻辑
     *
     * @param OnGrabBeforeTag 在抓取前激活的 Tag
     * @param OnGrabAfterTag 在抓取后激活的 Tag
     */
    void SkillActivateLogic(const FGameplayTag &OnGrabBeforeTag, const FGameplayTag &OnGrabAfterTag);

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

    UPROPERTY(EditDefaultsOnly, Category = "Noyta | Tag")
    FGameplayTag Skill_1_OnGrabBeforeTag;

    UPROPERTY(EditDefaultsOnly, Category = "Noyta | Tag")
    FGameplayTag Skill_1_OnGrabAfterTag;

    UPROPERTY(EditDefaultsOnly, Category = "Noyta | Tag")
    FGameplayTag Skill_2_OnGrabBeforeTag;

    UPROPERTY(EditDefaultsOnly, Category = "Noyta | Tag")
    FGameplayTag Skill_2_OnGrabAfterTag;

    UPROPERTY(EditDefaultsOnly, Category = "Noyta | Tag")
    FGameplayTag Skill_3_OnGrabBeforeTag;

    UPROPERTY(EditDefaultsOnly, Category = "Noyta | Tag")
    FGameplayTag Skill_3_OnGrabAfterTag;
};