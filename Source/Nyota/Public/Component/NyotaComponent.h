// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "DataAsset/NyotaCharacterConfig.h"
#include "Projectile/ProjectileBase.h"
#include "Ability/GA/GA_Base.h"
#include "Component/Nyota_AbilitySystemComponent.h"

#include "NyotaComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NYOTA_API UNyotaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UNyotaComponent();

//*****************************//
//** 预设的GA 和 GE 必须要填 **//
// ****************************//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterDefaultConfig, Replicated)
	UNyotaCharacterConfig* CharacterConfig;

public:

	//get character weapon instance
	//return null when not owning weapon
	UFUNCTION(BlueprintCallable)
	ANyotaWeapon* GetWeaponInstance();

public:

	//get character on hit projectile information
	UPROPERTY(BlueprintReadOnly, Replicated)
	FProjectileCFG CurrentActivateProjectileInfo;

protected:
	// current on hand weapon
	UPROPERTY(Replicated)
	ANyotaWeapon* CurrentWeapon;


private:
	// read the config file and initilize the weapon
	bool InitializeWeapon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



#pragma region CharacterCombo


private:

	friend class UAN_CombStart;
	friend class UAN_CombEnd;
	friend class UGA_Attack;


#pragma region MeleeAttack
public:

public:

	/** The combo index for the currently active combo */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "GAS Companion|Combo")
	int32 ComboIndex = 0;

	/** Whether or not the combo window is opened (eg. player can queue next combo within this window) */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "GAS Companion|Combo")
	bool bComboWindowOpened = false;

	/** Should we queue the next combo montage for the currently active combo */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "GAS Companion|Combo")
	bool bShouldTriggerCombo = false;

	/** Should we trigger the next combo montage */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "GAS Companion|Combo")
	bool bRequestTriggerCombo = false;

	/** Should we trigger the next combo montage */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "GAS Companion|Combo")
	bool bNextComboAbilityActivated = false;

	/** Reference to GA_GSC_Melee_Base */
	TSubclassOf<UGA_Base> MeleeBaseAbility;



	UFUNCTION(BlueprintCallable)
	void DoMeleeAttack(TSubclassOf<UGameplayAbility> AttackAbility);


#pragma endregion

};

