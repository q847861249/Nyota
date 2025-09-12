// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// 修改：添加引用
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Component/Nyota_AbilitySystemComponent.h"
#include "InputActionValue.h"
#include "DataAsset/NyotaCharacterConfig.h"
#include "Character/NyotaPlayerController.h"
#include "Character/NyotaMovementComponent.h"

#include "GameplayEffectExtension.h"
#include "NyotaCharacters.generated.h"



class UNyotaAttributeSet;
class UNyotaMovementComponent;
class UNyota_AbilitySystemComponent;


UCLASS()
class NYOTA_API ANyotaCharacters : public ACharacter,
	public IAbilitySystemInterface //修改：继承接口
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties

	ANyotaCharacters(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UNyotaMovementComponent* CharacterMovementComponent;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(BlueprintReadWrite)
	ANyotaPlayerController* NyotaPlayerController;

	// 修改：申明ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities)
	TObjectPtr<UNyota_AbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<UNyotaAttributeSet>AttributeSet;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = NyotaCharacterBaseSetting, meta = (AllowPrivateAccess = "true"))
	class UNyotaComponent* NyotaComponent;

	UFUNCTION()
	virtual UNyota_AbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	bool ApplyGameplayEffectToself(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle inEffectHandle);

	UFUNCTION()
	UNyotaAttributeSet* getAttributeSet() const { return AttributeSet; }



	/** Default NetWork override function */
	UFUNCTION()
	virtual void PossessedBy(AController* NewController) override;

	/** Default GAS Start Effect Setup */
	UFUNCTION()
	void ApplyStartUpEffect();

	/** Send GameplayEvent To Self */
	UFUNCTION()
	void SendGameEventByTag(FGameplayTag Tag);


	/** 修改：给Ability数组 */
	void GiveAbility();

	/** 修改：激活预载的Ability 以标签的形式 */
	UFUNCTION(BlueprintCallable)
	bool TryActiveAbilityByTag(FGameplayTag Tag);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	UFUNCTION(BlueprintCallable, reliable,Server)
	void EnableRagDoll();


	UFUNCTION(reliable, NetMulticast)
	void Rep_EanbleRagdoll_Multicast();


	void StartRagDoll();


	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	
protected:

	UFUNCTION()
	void OnRagdollStateChanged(const FGameplayTag CallbackTag, int32 NewCount);

public:

	UFUNCTION(BlueprintCallable)
	void PrintString(const FString& string, FLinearColor TextColor = FLinearColor(0.0f, 0.66f, 1.0f), float duration = 0.2f);





private:

	friend class NyotaPlayer;
};
