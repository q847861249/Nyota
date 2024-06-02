// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// 修改：添加引用
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "InputActionValue.h"
#include "DataAsset/NyotaCharacterConfig.h"
#include "Character/NyotaPlayerController.h"

#include "NyotaCharacters.generated.h"


class UNyotaAttributeSet;

UCLASS()
class NYOTA_API ANyotaCharacters : public ACharacter,
	public IAbilitySystemInterface //修改：继承接口
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANyotaCharacters();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(BlueprintReadWrite)
	ANyotaPlayerController* NyotaPlayerController;

	// 修改：申明ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = NyotaCharacterBaseSetting, meta = (AllowPrivateAccess = "true"))
	class UNyotaComponent* NyotaComponent;

	UPROPERTY()
	TObjectPtr<UNyotaAttributeSet>AttributeSet;

	UFUNCTION(BlueprintCallable)
	bool ApplyGameplayEffectToself(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle inEffectHandle);

	UNyotaAttributeSet* getAttributeSet() const { return AttributeSet; }

	// 修改：实现接口方法
	UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	/** Default NetWork override function */
	virtual void PossessedBy(AController* NewController) override;

	/** Default GAS Start Effect Setup */
	void ApplyStartUpEffect();

	/** Send GameplayEvent To Self */
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

private:

	friend class NyotaPlayer;
};
