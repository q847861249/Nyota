// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "JsEnv.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NyotaGameInstance.generated.h"

/**
 *
 */
UCLASS()
class NYOTA_API UNyotaGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    virtual void OnStart() override;

    virtual void Shutdown() override;

    void EnableJsEnv();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puerts")
    bool bEnableDebugger = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puerts")
    bool bWaitDebugger = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puerts")
    int DebuggerPort = 8889;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puerts")
    FString ModuleName = "Main";

private:
    TSharedPtr<puerts::FJsEnv> GameScript;
};
