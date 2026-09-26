// Fill out your copyright notice in Description page of Project Settings.

#include "AbilitySystem/Abilities/Player/GA_Charge.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Tasks/AbilityTask_EveryFrameTick.h"
#include "Character/BaseCharacter.h"
#include "Character/NyotaStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/GameTags.h"

namespace
{
    /** 推人距离钳制的最小安全间隙，单位：cm：敌人胶囊与野猪胶囊之间至少保留的空隙，防止重叠触发穿透解算互推。 */
    constexpr float BulldozeMinGap = 10.f;
}

UGA_Charge::UGA_Charge(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    // 派生普攻通过该 Tag 查找正在运行的冲锋实例，因此必须在 C++ 侧固定写入
    AbilityTags.AddTag(Nyota::Ability_Charge);

    // 可用性表：冲锋/推土机中技能一不可用（不能重复冲锋）
    ActivationBlockedTags.AddTag(Nyota::Ability_State_Charging);
}

void UGA_Charge::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    StartCharging();
}

void UGA_Charge::StartCharging()
{
    ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    UCharacterMovementComponent *Movement = IsValid(Character) ? Character->GetCharacterMovement() : nullptr;

    if (!IsValid(Movement))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

        return;
    }

    // 冲锋期间免疫所有控制效果：挂 Charging（供 HeroComponent 降转向、派生窗口判断）
    // 与 Immune_Control（ApplyStatusEffect 统一检查的免疫标记）
    TaggedAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

    if (IsValid(TaggedAbilitySystemComponent))
    {
        TaggedAbilitySystemComponent->AddLooseGameplayTag(Nyota::Ability_State_Charging);
        TaggedAbilitySystemComponent->AddLooseGameplayTag(Nyota::Status_Immune_Control);

        bImmunityTagGranted = true;
    }

    // 抬高 MaxWalkSpeed：LaunchCharacter 写入的速度在 Walking 模式下会被该值钳制。
    // 转向降速由 UNyotaHeroComponent 在 TickComponent 中检测 Charging Tag 后切换插值速度实现
    SavedMaxWalkSpeed = Movement->MaxWalkSpeed;
    Movement->MaxWalkSpeed = ChargeSpeed;

    bChargeStarted = true;

    // 逐帧驱动冲锋：UAbilityTask_EveryFrameTick 激活后由 ASC（UGameplayTasksComponent）每帧回调一次，
    // 技能结束/取消时 GAS 自动销毁任务并停止回调，无需手动管理计时器。
    // 注意不能用 AddMovementInput + 固定周期定时器：输入只在施加的那一帧生效，
    // 高帧率下两次定时器之间会出现多个"无输入帧"被制动减速，导致帧率越高移速越慢
    ChargeTickTask = UAbilityTask_EveryFrameTick::EveryFrameTick(this);
    ChargeTickTask->OnEveryFrameTick.AddDynamic(this, &ThisClass::OnChargeTick);

    // 兜底最大时长，防止地形异常导致无限冲锋
    GetWorld()->GetTimerManager().SetTimer(
        MaxDurationTimerHandle,
        FTimerDelegate::CreateWeakLambda(
            this,
            [this]() {
                FinishCharge();
            }
        ),
        MaxChargeDuration,
        false
    );

    OnChargeStart_Visual();
}

void UGA_Charge::OnChargeTick(float DeltaTime)
{
    ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    UWorld *World = GetWorld();

    if (!IsValid(Character) || !IsValid(World))
    {
        return;
    }

    // 逐帧直接写入速度：Velocity 每 Tick 被覆写，移动结果与帧率无关（位移 = 速度 × DeltaTime）
    Character->AddMovementInput(Character->GetActorForwardVector() * ChargeSpeed, true, false);

    const FVector Start = Character->GetActorLocation();
    const FVector Forward = Character->GetActorForwardVector().GetSafeNormal2D();

    //--------------------------------
    // 推土机状态：从敌人身位检测墙体，再把敌人钉到野猪正前方
    //--------------------------------

    if (ABaseCharacter *Bulldozed = BulldozedEnemy.Get(); IsValid(Bulldozed))
    {
        // 墙体检测以"敌人前缘"为准：敌人被推到即将贴墙时结束冲锋，而不是野猪离墙很远就提前结束
        float EnemyRadius = 50.f;

        if (const UCapsuleComponent *EnemyCapsule = Bulldozed->GetCapsuleComponent(); IsValid(EnemyCapsule))
        {
            EnemyRadius = EnemyCapsule->GetScaledCapsuleRadius();
        }

        const FVector EnemyLocation = Bulldozed->GetActorLocation();

        FCollisionQueryParams WallParams;
        WallParams.AddIgnoredActor(Character);
        WallParams.AddIgnoredActor(Bulldozed);

        TArray<FHitResult> WallHits;
        World->SweepMultiByChannel(
            WallHits, EnemyLocation, EnemyLocation + Forward * (EnemyRadius + 10.f), FQuat::Identity, ECC_Pawn,
            FCollisionShape::MakeSphere(10.f), WallParams
        );

        for (const FHitResult &Hit : WallHits)
        {
            // 只把"法线足够陡的非角色命中"视为墙，过滤掉脚下的地面与缓坡
            if (IsValid(Cast<ABaseCharacter>(Hit.GetActor())) || !IsWallHit(Hit))
            {
                continue;
            }

            HandleWallImpact();

            return;
        }

        // 使用钳制后的推人距离：过小的配置值会把敌人钉进野猪胶囊内部，触发穿透解算互推
        const FVector TargetLocation = Start + Forward * EffectiveBulldozeForwardOffset + FVector(0.f, 0.f, BulldozeZOffset);
        Bulldozed->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::ResetPhysics);

        return;
    }

    //--------------------------------
    // 自由冲锋：前向扫掠，按距离顺序处理第一个命中的敌人或墙体
    //--------------------------------

    const float SweepDistance = HitRadius + ChargeSpeed * DeltaTime * 2.f;
    const FVector End = Start + Forward * SweepDistance;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Character);

    TArray<FHitResult> Hits;
    World->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(HitRadius), QueryParams);

    if (bDrawDebugs)
    {
        DrawDebugCapsule(
            World, (Start + End) * 0.5f, FVector::Distance(Start, End) * 0.5f, HitRadius,
            FRotationMatrix::MakeFromX(End - Start).ToQuat(), FColor::Yellow, false, 0.f
        );
    }

    // SweepMultiByChannel 返回结果按命中距离排序，第一个有效命中决定结算
    for (const FHitResult &Hit : Hits)
    {
        ABaseCharacter *Enemy = Cast<ABaseCharacter>(Hit.GetActor());

        if (IsValid(Enemy))
        {
            if (Enemy == Character || !Enemy->IsAlive())
            {
                continue;
            }

            EnterBulldozeState(Enemy);

            return;
        }

        // 非角色、阻挡命中且法线足够陡（墙面而非地面/坡）→ 撞到障碍物（无敌人），野猪自身眩晕
        if (Hit.bBlockingHit && IsWallHit(Hit))
        {
            HandleWallImpact();

            return;
        }
    }
}

bool UGA_Charge::IsWallHit(const FHitResult &Hit) const
{
    // 命中法线无效时保守视为非墙，避免异常数据触发误判
    if (Hit.ImpactNormal.IsNearlyZero())
    {
        return false;
    }

    // 法线与竖直向上的夹角：地面≈0°、墙面≈90°，夹角越大表面越"陡"，才值得撞停
    const float AngleFromUp = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Hit.ImpactNormal.GetSafeNormal(), FVector::UpVector)));

    return AngleFromUp >= WallMinNormalAngleFromUp;
}

void UGA_Charge::EnterBulldozeState(ABaseCharacter *Enemy)
{
    if (IsValid(BulldozedEnemy.Get()))
    {
        return;
    }

    BulldozedEnemy = Enemy;

    //--------------------------------
    // 防反向奔跑（双保险）：
    // BulldozeForwardOffset 小于双方胶囊半径之和时，敌人会被每帧传进野猪胶囊内部，
    // 双方 CharacterMovement 的穿透解算沿最小分离向量互相推开，野猪被持续推向身后，
    // 表现为"反向奔跑"
    //--------------------------------

    // 保险一：把实际推人距离钳制到下限（双方半径之和 + 安全间隙），保证敌人始终在野猪胶囊外
    ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    float BoarRadius = 0.f;

    if (IsValid(Character))
    {
        if (const UCapsuleComponent *BoarCapsule = Character->GetCapsuleComponent(); IsValid(BoarCapsule))
        {
            BoarRadius = BoarCapsule->GetScaledCapsuleRadius();
        }
    }

    float EnemyRadius = 0.f;

    if (const UCapsuleComponent *EnemyCapsuleComp = Enemy->GetCapsuleComponent(); IsValid(EnemyCapsuleComp))
    {
        EnemyRadius = EnemyCapsuleComp->GetScaledCapsuleRadius();
    }

    EffectiveBulldozeForwardOffset = FMath::Max(BulldozeForwardOffset, BoarRadius + EnemyRadius + BulldozeMinGap);

    // 保险二：推人期间双方互相忽略碰撞查询。即使因帧率波动导致胶囊短暂重叠，
    // 穿透解算与前向移动扫掠也不会再把野猪往后推/挡住；结束时在 ReleaseBulldozedEnemy 中恢复。
    // 注意：组件 MoveIgnore 只影响组件自身的移动查询，技能里显式构造的 FCollisionQueryParams 扫掠不受影响，
    // 因此墙体检测、派生攻击等手动扫掠仍能正常命中被推的敌人
    if (IsValid(Character))
    {
        Character->MoveIgnoreActorAdd(Enemy);
        Enemy->MoveIgnoreActorAdd(Character);
    }

    // 冻结敌人：只授予屏蔽维度 Tag，实际的锁移动/停 AI 由敌人身上的 UNyotaStatusComponent 反应执行
    if (UAbilitySystemComponent *EnemyASC = Enemy->GetAbilitySystemComponent(); IsValid(EnemyASC))
    {
        EnemyASC->AddLooseGameplayTag(Nyota::Status_Blocked_Movement);
        EnemyASC->AddLooseGameplayTag(Nyota::Status_Blocked_AI);
        EnemyASC->AddLooseGameplayTag(Nyota::Status_Blocked_Ability);

        bEnemyBlockedTagsGranted = true;
    }

    // 推人期间把敌人胶囊改为仅查询：否则敌人胶囊会物理阻挡野猪前进，推不动
    if (UCapsuleComponent *EnemyCapsule = Enemy->GetCapsuleComponent(); IsValid(EnemyCapsule))
    {
        EnemyCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }

    // 野猪侧挂推土机状态 Tag，作为派生普攻的激活前置条件
    if (IsValid(TaggedAbilitySystemComponent))
    {
        TaggedAbilitySystemComponent->AddLooseGameplayTag(Nyota::Ability_State_Charging_Bulldozer);
    }

    OnBulldozeStart_Visual();
}

void UGA_Charge::HandleWallImpact()
{
    if (bWallImpacted)
    {
        return;
    }

    bWallImpacted = true;

    ABaseCharacter *Bulldozed = BulldozedEnemy.Get();

    if (IsValid(Bulldozed))
    {
        // 撞到障碍物（有敌人）：先解除推土机绑定，再施加定时眩晕（GE 到期自动恢复）
        ReleaseBulldozedEnemy();
        UNyotaStatusComponent::ApplyStatusEffect(Bulldozed, StunStatusEffect, EnemyStunDuration);
    }
    else if (ABaseCharacter *Owner = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()); IsValid(Owner))
    {
        // 撞到障碍物（无敌人）：野猪自身被眩晕（自我惩罚）。
        // 必须先移除免疫 Tag，否则会被 ApplyStatusEffect 的免疫检查拦截
        if (IsValid(TaggedAbilitySystemComponent) && bImmunityTagGranted)
        {
            TaggedAbilitySystemComponent->RemoveLooseGameplayTag(Nyota::Status_Immune_Control);

            bImmunityTagGranted = false;
        }

        UNyotaStatusComponent::ApplyStatusEffect(Owner, StunStatusEffect, SelfStunDuration);
    }

    OnSmashWall_Visual();

    FinishCharge();
}

void UGA_Charge::ReleaseBulldozedEnemy()
{
    ABaseCharacter *Bulldozed = BulldozedEnemy.Get();

    if (!IsValid(Bulldozed))
    {
        return;
    }

    // 移除屏蔽维度 Tag，UNyotaStatusComponent 监听到计数归零后自动恢复移动与 AI
    if (bEnemyBlockedTagsGranted)
    {
        if (UAbilitySystemComponent *EnemyASC = Bulldozed->GetAbilitySystemComponent(); IsValid(EnemyASC))
        {
            EnemyASC->RemoveLooseGameplayTag(Nyota::Status_Blocked_Movement);
            EnemyASC->RemoveLooseGameplayTag(Nyota::Status_Blocked_AI);
            EnemyASC->RemoveLooseGameplayTag(Nyota::Status_Blocked_Ability);
        }

        bEnemyBlockedTagsGranted = false;
    }

    if (UCapsuleComponent *EnemyCapsule = Bulldozed->GetCapsuleComponent(); IsValid(EnemyCapsule))
    {
        EnemyCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    // 恢复双方碰撞忽略：EnterBulldozeState 里推人期间互相 MoveIgnore 过
    if (ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()); IsValid(Character))
    {
        Character->MoveIgnoreActorRemove(Bulldozed);
        Bulldozed->MoveIgnoreActorRemove(Character);
    }

    BulldozedEnemy.Reset();
}

void UGA_Charge::LaunchBulldozedEnemy(ABaseCharacter *EnemyToLaunch)
{
    if (!IsValid(EnemyToLaunch) || EnemyToLaunch != BulldozedEnemy.Get())
    {
        return;
    }

    ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

    if (!IsValid(Character))
    {
        return;
    }

    // 先解除推土机绑定（恢复移动模式），再施加击飞初速度
    ReleaseBulldozedEnemy();

    FVector LaunchVelocity = Character->GetActorForwardVector() * LaunchForce;
    LaunchVelocity.Z += LaunchZOffset;

    if (ACharacter *EnemyCharacter = Cast<ACharacter>(EnemyToLaunch); IsValid(EnemyCharacter))
    {
        EnemyCharacter->LaunchCharacter(LaunchVelocity, true, true);
    }

    FinishCharge();
}

void UGA_Charge::FinishCharge()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Charge::EndAbility(
    const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled
)
{
    // 逐帧任务无需手动停止：EndAbility 时 GAS 会自动销毁本技能的所有 AbilityTask
    if (IsValid(GetWorld()))
    {
        GetWorld()->GetTimerManager().ClearTimer(MaxDurationTimerHandle);
    }

    // 技能结束但敌人仍被推着走时（如被外力打断），兜底释放敌人
    ReleaseBulldozedEnemy();

    // 恢复野猪的移动设置
    if (bChargeStarted)
    {
        if (ACharacter *Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()); IsValid(Character))
        {
            if (UCharacterMovementComponent *Movement = Character->GetCharacterMovement(); IsValid(Movement))
            {
                Movement->MaxWalkSpeed = SavedMaxWalkSpeed;
            }
        }

        bChargeStarted = false;
    }

    // 移除冲锋状态 Tag（派生窗口、转向降速、免疫控制都依赖它们）
    if (IsValid(TaggedAbilitySystemComponent))
    {
        TaggedAbilitySystemComponent->RemoveLooseGameplayTag(Nyota::Ability_State_Charging);
        TaggedAbilitySystemComponent->RemoveLooseGameplayTag(Nyota::Ability_State_Charging_Bulldozer);

        // 免疫 Tag 可能已在撞墙自罚路径提前移除，用计数标记避免重复移除导致 Tag 计数错乱
        if (bImmunityTagGranted)
        {
            TaggedAbilitySystemComponent->RemoveLooseGameplayTag(Nyota::Status_Immune_Control);

            bImmunityTagGranted = false;
        }
    }

    BulldozedEnemy.Reset();
    TaggedAbilitySystemComponent = nullptr;
    bWallImpacted = false;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
