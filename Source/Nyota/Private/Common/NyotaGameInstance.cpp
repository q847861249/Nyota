// Fill out your copyright notice in the Description page of Project Settings.

#include "Common/NyotaGameInstance.h"

void UNyotaGameInstance::Init()
{
    Super::Init();

    EnableJsEnv();
}

void UNyotaGameInstance::OnStart()
{
    Super::OnStart();
}

void UNyotaGameInstance::Shutdown()
{
    Super::Shutdown();

    GameScript.Reset();
}

void UNyotaGameInstance::EnableJsEnv()
{
    if (!bEnableDebugger)
    {
        GameScript = MakeShared<puerts::FJsEnv>(TEXT("TypeScript"));
    }
    else
    {
        GameScript = MakeShared<puerts::FJsEnv>(
            std::make_unique<puerts::DefaultJSModuleLoader>(TEXT("TypeScript")),
            std::make_shared<puerts::FDefaultLogger>(),
            DebuggerPort
        );
    }

    if (bWaitDebugger)
    {
        GameScript->WaitDebugger(); // 等待调试器连接
    }

    TArray<TPair<FString, UObject *>> Arguments;
    Arguments.Add(TPair<FString, UObject *>(TEXT("GameInstance"), this));
    GameScript->Start(ModuleName, Arguments);
}