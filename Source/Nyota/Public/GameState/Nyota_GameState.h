// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Nyota_GameState.generated.h"

/**
 * 
 */
class UMainUI;
UCLASS()
class NYOTA_API ANyota_GameState : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly,Category="Time")
	int32 TimeSetting;

	UPROPERTY(ReplicatedUsing = OnRep_RemainTime)
	int32 RemainTime;

    FTimerHandle GameTimerHandle;

	TObjectPtr<UMainUI> MainUI;

	UFUNCTION()
	void OnRep_RemainTime();

	UFUNCTION()
	void UpdateTime();



protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
    virtual void RemovePlayerState(APlayerState* PlayerState) override;
};
