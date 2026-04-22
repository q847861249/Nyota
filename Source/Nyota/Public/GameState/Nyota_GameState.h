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
	/**This variable determines the upper limit of RemainTime. */
	UPROPERTY(EditDefaultsOnly,Category="Time")
	int32 TimeSetting;
	/** The remaining time variable decreases by one per second. */
	UPROPERTY(ReplicatedUsing = OnRep_RemainTime)
	int32 RemainTime;

    FTimerHandle GameTimerHandle;
	/** The reference of MainUI */
	TObjectPtr<UMainUI> MainUI;
	/** Synchronous remian time to client */
	UFUNCTION()
	void OnRep_RemainTime();
	/**Update Remain time on server side */
	UFUNCTION()
	void UpdateTime();



protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
    virtual void RemovePlayerState(APlayerState* PlayerState) override;
};
