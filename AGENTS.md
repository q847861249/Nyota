# AGENTS.md

本文件定义本项目（QinziMMO）中 AI 助手与开发者写代码时应遵守的约定。

## 回复语言

回复内容时，尽量通俗易懂。

## Unreal Engine MCP（默认启用）

- 项目 MCP 配置：`.cursor/mcp.json`（`unreal-engine` → `http://127.0.0.1:8080/mcp`）。
- 处理蓝图、关卡、资产、编辑器验证时，**优先通过 UE MCP**；会话开始先用 `GetDynamicTools` 确认已连接。
- MCP 未连接时：告知用户打开 UE 编辑器并启动 MCP 服务，再回退 C++ 或手动蓝图方案。

## 代码注释规范（必须遵守）

在本项目中新增或修改代码时，必须补充注释。注释统一使用 **Doxygen 风格**，并用中文写清楚意图。

### 必须注释的内容

- **类 / 结构体 / 接口**：说明它负责什么、在整体架构里扮演什么角色。
- **函数 / 方法**：说明它处理什么逻辑、何时调用、关键前置条件或副作用。
- **变量 / 成员变量 / 重要局部变量**：说明它的作用、含义，以及必要时的取值范围或单位。
- **枚举 / 常量**：说明业务含义，不要只重复名字。

### 注释写法要求

- 优先解释“为什么这样做 / 它解决什么问题”，不要只复述代码表面动作。
- 注释要通俗、具体，方便后续维护。
- 避免无意义注释，例如“获取指针”“返回结果”。
- 不要给每一行都堆注释；重点说明类、函数、关键变量和复杂逻辑。
- **函数注释写在头文件（`.h`）的声明处**，不要把完整接口说明重复写到 `.cpp` 实现里；`.cpp` 里只补充实现细节说明。

### Doxygen 风格示例

类注释：

```cpp
/**
 * @brief 管理玩家战斗属性与伤害结算入口。
 * @details 负责把 GAS 属性变化同步到 UI，并处理受击后的反馈逻辑。
 */
class UQinziCombatComponent : public UActorComponent
{
};
```

函数注释（写在头文件声明处）：

```cpp
// XxxComponent.h
/**
 * @brief 处理一次受击后的伤害结算。
 * @param DamageAmount 本次结算伤害值。
 * @param HitResult 受击命中信息，用于反馈位置与方向。
 * @return true 表示伤害已成功结算；false 表示被无敌或过滤规则拦截。
 */
bool ApplyHitDamage(float DamageAmount, const FHitResult& HitResult);
```

变量注释：

```cpp
/** 当前锁定的目标 Actor；为空表示没有锁定目标。 */
TWeakObjectPtr<AActor> LockedTarget;

/** 普攻冷却剩余时间，单位：秒。 */
float AttackCooldownRemaining = 0.f;
```

### 代码格式化

- 参考项目根目录下的`.clang-format`
