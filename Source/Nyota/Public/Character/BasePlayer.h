// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Enums/PlayerType.h"

#include "BasePlayer.generated.h"

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

    void SetGrabbedEnemy(ABaseCharacter *Character);

    ABaseCharacter *GetGrabbedEnemy() const;

    void ResetGrabbedEnemy();

    /** 子技能（Slam/PressureBlast等）在触发 GrabEnd 前设定，GA_Grab::OnGrabTimeout 读取。<=0 时使用 GA_Grab 默认 ThrownForce */
    float PendingThrownForce = -1.f;

    UFUNCTION(BlueprintCallable, Category = "Nyota | Character")
    UNyotaAbilitySystemComponent *GetNyotaAbilitySystemComponent() const;

protected:
    /**玩家角色的捕获体触发碰撞后执行 */
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult
    );

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputMappingContext *InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputAction *LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
    UInputAction *MoveAction;

    float TargetYaw; // 记录目标朝向

    virtual void OnAbilitySystemInitialized();

    virtual void OnAbilitySystemUninitialized();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Tag")
    FGameplayTag Skill_1_Tag;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Tag")
    FGameplayTag Skill_2_Tag;

    UPROPERTY(EditDefaultsOnly, Category = "Nyota | Tag")
    FGameplayTag Skill_3_Tag;

    TWeakObjectPtr<ABaseCharacter> GrabbedEnemy;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyota | Character", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNyotaPawnExtensionComponent> PawnExtensionComponent;
};