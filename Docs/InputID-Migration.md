# 技能输入系统重构方案：从 Tag 激活迁移到 InputID 激活

## 1. 当前架构概述

### 输入流程

```
蓝图 InputAction (Started)
    → BasePlayer::OnSkill_X_Started()          // BlueprintCallable
        → ASC->TryActivateAbilitiesByTag(Tag)  // 通过标签查找 GA
            → TryActivateAbility(Handle)        // 激活检查（标签条件、冷却等）
```

### 当前问题

- `UAbilityTask_WaitInputRelease` **无法工作** — 该 Task 依赖 `ASC->AbilityInputPressed(InputID)` 设置内部 `InputPressed` 标记，但 Tag 激活路径完全绕过了这套机制
- 需要额外创建自定义 AbilityTask（如 `UAbilityTask_WaitInputActionReleased`）来桥接增强输入系统
- 每个需要"按下/松开"语义的技能都要额外配置 `InputAction` 引用，增加蓝图布线成本

---

## 2. InputID 方案概述

### 核心思想

将 GAS 内置的 `AbilityInputPressed` / `AbilityInputReleased` 管道接入增强输入系统，替代 `TryActivateAbilitiesByTag` 的角色。

### 新输入流程

```
蓝图 InputAction (Started / Completed)
    → BasePlayer::OnInputAction_Started(InputID) / OnInputAction_Completed(InputID)
        → ASC->AbilityInputPressed(InputID)      // 激活匹配的 GA
        → ASC->AbilityInputReleased(InputID)     // 释放匹配的 GA
            → ASC 内部设置 InputPressed 标记
            → UAbilityTask_WaitInputRelease 原生工作 ✅
```

---

## 3. Tag 与 InputID 的关系

### 三句话总结

1. **Tag 负责"能不能放"** — `BlockAbilitiesWithTag`、`CancelAbilitiesWithTag`、`ActivationRequiredTags`、`ActivationBlockedTags`、冷却、消耗……全部由 Tag 控制
2. **InputID 负责"哪个按键放"** — 按下键 1 → `AbilityInputPressed(1)` → 激活 `InputID=1` 的 GA
3. **两者完全正交** — 换成 InputID 激活后，`TryActivateAbility` 内部的所有标签检查**照常执行，一行都不会少**

### 标签在蓝图 GA 中仍然生效的清单

| 蓝图配置 | 作用 | InputID 下是否生效 |
|---|---|---|
| `Ability Tags` | GA 的身份标签 | ✅ 照常 |
| `Cancel Abilities With Tag` | 激活时取消带此 Tag 的技能 | ✅ 照常 |
| `Block Abilities With Tag` | 激活期间阻塞带此 Tag 的技能 | ✅ 照常 |
| `Activation Required Tags` | 拥有者必须有的 Tag | ✅ 照常 |
| `Activation Blocked Tags` | 拥有者不能有的 Tag | ✅ 照常 |
| `Source Required Tags` | 来源必须有的 Tag | ✅ 照常 |
| `Source Blocked Tags` | 来源不能有的 Tag | ✅ 照常 |
| `Activation Owned Tags` | 激活时给自身加的 Tag | ✅ 照常 |
| `Cooldown Gameplay Effect` | 冷却 | ✅ 照常 |
| `Cost Gameplay Effect` | 消耗 | ✅ 照常 |

### 原理

`ASC->AbilityInputPressed(InputID)` 内部流程：

```cpp
// UE 引擎源码简化版
void UAbilitySystemComponent::AbilityInputPressed(int32 InputID)
{
    for (FGameplayAbilitySpec& Spec : ActivatableAbilities)
    {
        if (Spec.InputID == InputID)          // <-- 通过 InputID 找到候选 GA
        {
            Spec.InputPressed = true;          // <-- 设置标记，WaitInputRelease 靠它工作
            TryActivateAbility(Spec.Handle);   // <-- 内部仍然检查全部 Tag 条件！
        }
    }
}
```

`TryActivateAbility` 内部会依次检查：
1. `ActivationRequiredTags` — 拥有者是否有这些 Tag
2. `ActivationBlockedTags` — 拥有者是否有这些 Tag（阻塞）
3. `SourceRequiredTags` / `SourceBlockedTags`
4. `BlockAbilitiesWithTag` — 当前激活的 GA 是否阻塞了这个 Tag
5. 冷却 GE 是否在生效
6. 消耗 GE 是否付得起

这些检查**与被谁调用无关**，无论是 `TryActivateAbilitiesByTag` 还是 `AbilityInputPressed` 调用 `TryActivateAbility`，走的都是同一条路径。

---

## 4. 实施步骤

### 4.1 定义 InputID 枚举

新增文件 `Source/Nyota/Public/Enums/AbilityInputID.h`：

```cpp
#pragma once

#include "CoreMinimal.h"

/**
 * 技能输入 ID，用于 GAS 的 AbilityInputPressed/Released 管道。
 * 每个 ID 对应一个输入槽位，由蓝图的 InputAction 绑定。
 * GA 蓝图中通过 AbilityInputID 属性关联。
 */
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
    None        = 0  UMETA(Hidden),
    LightAttack = 1,
    LeftLightAttack = 2,
    RightLightAttack = 3,
    Skill1      = 4,
    Skill2      = 5,
    Skill3      = 6,
    Jump        = 7,
    Dash        = 8,
    // 按需扩展...
};
```

> **设计说明：** 使用 `uint8` 枚举。GAS 的 `InputID` 是 `int32`，但实际游戏不需要 40 亿个按键。`uint8` 最多 255 个足够，且更节约网络带宽。

### 4.2 修改 `BasePlayer` — 简化输入处理

#### `BasePlayer.h` 变更

```cpp
// ===== 保留的 =====
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputMappingContext *InputMappingContext;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *LookAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *MoveAction;

// ===== 新增：每个 InputAction 直接绑定到 InputID =====
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *LightAttackAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *LeftLightAttackAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *RightLightAttackAction;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *Skill1Action;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *Skill2Action;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nyota | Input")
UInputAction *Skill3Action;

// ===== 新增：通用输入处理方法 =====
void BindAbilityInput(UInputAction *Action, EAbilityInputID InputID);
void OnAbilityInputStarted(EAbilityInputID InputID);
void OnAbilityInputCompleted(EAbilityInputID InputID);

// ===== 可删除的 =====
// 以下方法可以删除（被 OnAbilityInputStarted/Completed 替代）：
// OnLightAttack_Started()
// OnLeftLightAttack_Started()
// OnRightLightAttack_Started()
// OnSkill_1_Started() / OnSkill_1_Completed()
// OnSkill_2_Started() / OnSkill_2_Completed()
// OnSkill_3_Started() / OnSkill_3_Completed()

// ===== 可删除的 =====
// 以下 Tag 属性不再需要（GA 通过 InputID 关联，不再需要 Tag 匹配）：
// Skill_1_Tag
// Skill_2_Tag
// Skill_3_Tag
```

#### `BasePlayer.cpp` 变更

```cpp
void ABasePlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // ... 现有 IMC 和 Move/Look 绑定保持不变 ...

    if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 绑定所有技能输入
        BindAbilityInput(LightAttackAction,     EAbilityInputID::LightAttack);
        BindAbilityInput(LeftLightAttackAction,  EAbilityInputID::LeftLightAttack);
        BindAbilityInput(RightLightAttackAction, EAbilityInputID::RightLightAttack);
        BindAbilityInput(Skill1Action,           EAbilityInputID::Skill1);
        BindAbilityInput(Skill2Action,           EAbilityInputID::Skill2);
        BindAbilityInput(Skill3Action,           EAbilityInputID::Skill3);

        // Move / Look 绑定保持不变
        if (LookAction)
        {
            EnhancedInputComponent->BindAction(
                LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::LookInput);
        }
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(
                MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::MoveInput);
        }
    }
}

void ABasePlayer::BindAbilityInput(UInputAction *Action, EAbilityInputID InputID)
{
    if (!Action)
    {
        return;
    }

    UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    if (!EnhancedInputComponent)
    {
        return;
    }

    const int32 ID = static_cast<int32>(InputID);

    // Started → 按键按下
    EnhancedInputComponent->BindAction(
        Action, ETriggerEvent::Started, this,
        GET_FUNCTION_NAME_CHECKED(ABasePlayer, OnAbilityInputStarted),
        ID  // 作为额外参数传入
    );

    // Completed → 按键松开
    EnhancedInputComponent->BindAction(
        Action, ETriggerEvent::Completed, this,
        GET_FUNCTION_NAME_CHECKED(ABasePlayer, OnAbilityInputCompleted),
        ID
    );
}

void ABasePlayer::OnAbilityInputStarted(EAbilityInputID InputID)
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    ASC->AbilityInputPressed(static_cast<int32>(InputID));
}

void ABasePlayer::OnAbilityInputCompleted(EAbilityInputID InputID)
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    ASC->AbilityInputReleased(static_cast<int32>(InputID));
}
```

> **注意：** `BindAction` 带额外参数的写法在 UE 5.4 中支持。`GET_FUNCTION_NAME_CHECKED` 保证编译期函数名校验。如果 `BindAction` 的额外参数版本不兼容，可以使用 lambda 或为每个 ID 编写独立的 `Started/Completed` 方法。

### 4.3 修改 `GiveDefaultAbility` — 设置 InputID

当前 `BaseCharacter.cpp:35-61` 中 `GiveAbility` 没有设置 `InputID`。需要让 GA 类自身携带 InputID 信息。

**方案：GA 子类暴露 `InputID` 属性**

在 `GA_BaseSkill.h` 中添加：

```cpp
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyota | Input")
EAbilityInputID AbilityInputID = EAbilityInputID::None;
```

然后修改 `BaseCharacter::GiveDefaultAbility()`：

```cpp
void ABaseCharacter::GiveDefaultAbility()
{
    UAbilitySystemComponent *ASC = GetAbilitySystemComponent();
    if (!IsValid(ASC))
    {
        return;
    }

    for (const auto &AbilityClass : GAClass)
    {
        FGameplayAbilitySpec AbilitySpec(AbilityClass);
        
        // 从 GA CDO 读取 InputID
        if (const UGA_BaseSkill *SkillCDO = AbilityClass->GetDefaultObject<UGA_BaseSkill>())
        {
            AbilitySpec.InputID = static_cast<int32>(SkillCDO->AbilityInputID);
        }
        
        ASC->GiveAbility(AbilitySpec);

        const UGameplayAbility *AbilityObj = AbilityClass->GetDefaultObject<UGameplayAbility>();
        if (AbilityObj && AbilityObj->AbilityTags.HasTagExact(Nyota::Ability_init))
        {
            ASC->TryActivateAbility(AbilitySpec.Handle);
        }
    }
}
```

### 4.4 蓝图配置

1. **GA 蓝图**（如 `BP_GA_WaterBubble`）：
   - 在 "Nyota \| Input" 分类下设置 **`Ability Input ID`** = `Skill1`（或对应槽位）
   - `Ability Tags`、`Block Abilities With Tag` 等标签配置**完全不动**

2. **角色蓝图**（`BP_BasePlayer`）：
   - 在 "Nyota \| Input" 分类下设置 **`Skill1 Action`** = `IA_Skill_1`
   - 每个 InputAction 只需设置一次，所有映射到同一 InputID 的 GA 会共享

### 4.5 `WaitInputRelease` 原生工作

完成以上迁移后，`GA_WaterBubble.cpp` 中的代码可以直接恢复为：

```cpp
// 原生 GAS Task，无需任何自定义代码
UAbilityTask_WaitInputRelease *WaitRelease = 
    UAbilityTask_WaitInputRelease::WaitInputRelease(this);
WaitRelease->OnRelease.AddDynamic(this, &ThisClass::OnWaterBubbleEnd);
WaitRelease->ReadyForActivation();
```

原理：`AbilityInputPressed(InputID)` 会在对应 Spec 上设置 `InputPressed = true`，`AbilityInputReleased(InputID)` 设置 `InputPressed = false` 并通知活跃 GA 的 `WaitInputRelease` Task。

---

## 5. 与当前自定义 Task 方案的对比

| 维度 | 方案 A：自定义 AbilityTask | 方案 B：InputID 迁移 |
|------|---------------------------|---------------------|
| **C++ 改动量** | 少量（新增 1 个 Task 类） | 较多（修改 BasePlayer、GA_BaseSkill、GiveDefaultAbility） |
| **蓝图改动量** | 每个技能 GA 需配置 `InputAction` | 角色蓝图配置一次，GA 配置 `InputID` |
| **WaitInputRelease** | 不能使用，需自定义替代 | ✅ 原生工作 |
| **松键监听** | 每个需要松键的 GA 都要加 `InputAction` | 自动，无需每个 GA 额外配置 |
| **组合键/长按/双击** | 需为每种模式写新 Task | 利用 GAS 内置 `WaitInputPress`、`WaitInputRelease` 等 |
| **网络同步** | 需手动处理 | GAS 内置 InputID 同步 |
| **与引擎示例/文档一致** | 非标准路径 | ✅ 与 Lyra、ActionRPG 等官方示例一致 |
| **多 GA 共用一个按键** | ❌ 需要额外设计 | ✅ 天然支持（同一 InputID 对应多个 GA） |
| **架构统一性** | 混合使用 Tag 激活 + 自定义输入监听 | 统一的 InputID 管道，概念清晰 |

### 推荐策略

- **短期（当前）**：继续使用方案 A（已实现的自定义 AbilityTask），改动最小，立即可用
- **中期（下次重构窗口）**：迁移到方案 B，彻底解决输入体系分裂问题，为后续迭代打好基础

---

## 6. 迁移清单

如果决定迁移到 InputID 方案：

- [ ] 新增 `Source/Nyota/Public/Enums/AbilityInputID.h`，定义 InputID 枚举
- [ ] `GA_BaseSkill.h` 添加 `AbilityInputID` 属性
- [ ] `BaseCharacter::GiveDefaultAbility()` 从 GA CDO 读取 `InputID` 并设置到 Spec
- [ ] `BasePlayer.h` — 用 `InputAction` 属性替代 `Skill_X_Tag`
- [ ] `BasePlayer.cpp` — `SetupPlayerInputComponent` 中实现 `BindAbilityInput` 统一绑定
- [ ] `BasePlayer.cpp` — 删除 `OnLightAttack_Started()` 等旧的 Started/Completed 方法
- [ ] `BasePlayer.h` — 删除 `Skill_X_Tag` 属性
- [ ] `GA_WaterBubble.cpp` — 删除自定义 Task，恢复使用原生 `WaitInputRelease`
- [ ] 逐个更新 GA 蓝图：设置 `Ability Input ID` 替代之前的标签区分
- [ ] 角色蓝图：设置 `InputAction` 属性绑定到对应 InputID
- [ ] 测试所有技能：按下激活、松开终止、排斥/取消/阻塞标签仍然生效
- [ ] 网络测试：客户端输入 → 服务器正确响应