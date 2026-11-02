// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Enums/PlayerType.h"

#include "BasePlayer.generated.h"

class UPlayerCharacterDataAsset;
class UNyotaAbilitySystemComponent;
class UNyotaPawnExtensionComponent;
/**
 *
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base character pawn class used by this project."))
class NYOTA_API ABasePlayer : public ABaseCharacter
{
    GENERATED_BODY()

public:
    ABasePlayer(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

    virtual void PossessedBy(AController *NewController) override;

    virtual void OnRep_PlayerState() override;

    virtual UAttributeSet *GetAttributeSet() const override;

    virtual void BeginPlay() override;

    /** @brief 记录抓取对象并进入固定朝向移动，供后续附着回调读取目标。 */
    void SetGrabbedEnemy(ABaseCharacter *Character);

    ABaseCharacter *GetGrabbedEnemy() const;

    /** @brief 清除抓取对象并恢复抓取前的转向设置，可重复调用。 */
    void ResetGrabbedEnemy();

    /** @brief 目标引用失效时恢复移动设置，不干预技能释放和附着流程。 */
    void RefreshGrabMovement();

    /** @brief 是否正在使用抓取四向移动。 */
    bool IsGrabMovementActive() const { return bGrabMovementActive; }

    /** @brief 返回成功抓取时的水平朝向，作为固定的移动坐标系。 */
    FRotator GetGrabMovementRotation() const { return GrabMovementRotation; }

    UPlayerCharacterDataAsset *GetConfig();

    /** 子技能（Slam/PressureBlast等）在触发 GrabEnd 前设定，GA_Grab::OnGrabTimeout 读取。<=0 时使用 GA_Grab 默认
     * ThrownForce */
    float PendingThrownForce = -1.f;

    UFUNCTION(BlueprintCallable, Category = "Nyota | Character")
    UNyotaAbilitySystemComponent *GetNyotaAbilitySystemComponent() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputMappingContext *InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputAction *LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputAction *MoveAction;

    float TargetYaw; // 记录目标朝向

    virtual void OnAbilitySystemInitialized();

    virtual void OnAbilitySystemUninitialized();

    // 玩家角色配置表引用
    UPROPERTY(EditDefaultsOnly, Category = "Character Config")
    TObjectPtr<UPlayerCharacterDataAsset> PlayerCharacterDataAsset;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Tag")
    FGameplayTag Skill_1_Tag;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Tag")
    FGameplayTag Skill_2_Tag;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Tag")
    FGameplayTag Skill_3_Tag;

    /** 当前抓取目标；弱引用允许目标销毁后自动失效。 */
    TWeakObjectPtr<ABaseCharacter> GrabbedEnemy;
    /** 是否已保存并覆盖抓取期间的转向设置。 */
    bool bGrabMovementActive = false;
    /** 成功抓取时的水平朝向，单位为度。 */
    FRotator GrabMovementRotation = FRotator::ZeroRotator;
    /** 抓取前的移动方向转向、控制器期望转向及控制器偏航设置。 */
    bool bSavedOrientToMovement = false;
    bool bSavedControllerDesiredRotation = false;
    bool bSavedControllerYaw = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyota | Character", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNyotaPawnExtensionComponent> PawnExtensionComponent;
};