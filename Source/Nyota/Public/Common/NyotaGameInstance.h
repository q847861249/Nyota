// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NyotaGameInstance.generated.h"

/**
 * @brief 提供项目游戏实例的原生基类，承接跨关卡的全局逻辑。
 * @details 保留该类型以兼容现有游戏实例蓝图，生命周期由 UGameInstance 管理。
 */
UCLASS()
class NYOTA_API UNyotaGameInstance : public UGameInstance
{
    GENERATED_BODY()
};
