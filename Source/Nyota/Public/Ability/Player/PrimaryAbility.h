// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/BaseAbility.h"
#include "PrimaryAbility.generated.h"

USTRUCT(BlueprintType)
struct FAbilityMontageData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Thrust = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HitBoxRadius = 100.f;
};
UCLASS()
class NYOTA_API UPrimaryAbility : public UBaseAbility
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
    TArray<FAbilityMontageData> AbilityMontageDataArr;

    FName CurrentActiveSocket;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetPrimaryMontage(int32 Index);


	void DrawHitBoxOverlapDebugs(const TArray<AActor*>& FilterActor, const FVector& HitBoxLocation,float HitBoxRadius);

	UFUNCTION(BlueprintCallable)
	void SendHitReactEventToActor(TArray<AActor*> HitActors);
public:
	UFUNCTION(BlueprintCallable)
	void PrimaryAttack(int32 ComboIndex);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Combo")
	float ComboResetTime = 1.0f;

	UFUNCTION()
	int32 GetAbilityMontageDataArrNum();
};
