# 功能

## 2026-10-15
- 项目升级到 Unreal Engine 5.8，使用 5.8.3 验证；游戏与编辑器目标采用 V7 构建规则。
- 移除 ElectronicNodes 插件及项目引用，使用引擎默认的节点连线显示。
- 适配 GAS 资产标签、动态来源标签和 Actor 网络更新频率接口，修复输入映射代码的头文件依赖。
- 补齐掉落配置结构体默认值，修正旧技能组件类名和被动技能标签的资源重定向。
- 为苹果道具目录 `Content/Fab/Apple` 添加精确的打包允许项，避免与平台保留目录名冲突。
- 按资产实际类型修正 Experience 和 PawnData 的扫描规则，使独立运行时可以找到其主资产 ID。
- 构建脚本按 `.uproject` 的引擎版本自动查找安装位置，也支持 `-EnginePath` 显式指定目录。
  - 编辑器：`powershell -ExecutionPolicy Bypass -File Script/Build.ps1 -Target Editor`
  - 游戏：`powershell -ExecutionPolicy Bypass -File Script/Build.ps1 -Target Game`
- 验证通过：Win64 Editor/Game 编译、77 个项目蓝图编译、4 项 `Nyota.*` 自动化测试，以及 `NewMap` 的 Windows Development 打包和 DX12 300 帧启动检查。
  - 验证包入口：`Saved/Builds/UE58/Windows/Nyota.exe`。
  - 测试报告：`Saved/Automation/UE58/index.json`；运行日志：`Saved/Logs/Upgrade58-PackagedSmoke-Final.log`。

## 2026-05-23
- 添加一技能螃蟹水球发射技能
- 添加二技能螃蟹水泡发射技能
- 添加三技能螃蟹抓取技能
  - 可配合一技能使用砸地技能
  - 可配合二技能使用旋风钳技能
- 添加螃蟹左击平A
- 添加螃蟹左重击
- 添加螃蟹右重击
- 添加野猪受击反馈（不同角度）
- 添加角色血条UI
- 添加攻击后的血量及UI更新
- 添加螃蟹AI
  - 自动巡逻
  - 检测到目标主动攻击

# 修复
