// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// 修改：添加引用
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "PawnBase.generated.h"

UCLASS()
class NYOTA_API APawnBase : public APawn,
	public IAbilitySystemInterface //修改：继承接口
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 修改：申明ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	// 修改：实现接口方法
	UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	// 修改：声明Ability数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> MyAbilities;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
