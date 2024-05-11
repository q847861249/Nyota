// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// �޸ģ��������
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "PawnBase.generated.h"

UCLASS()
class NYOTA_API APawnBase : public APawn,
	public IAbilitySystemInterface //�޸ģ��̳нӿ�
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// �޸ģ�����ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	// �޸ģ�ʵ�ֽӿڷ���
	UAbilitySystemComponent* GetAbilitySystemComponent()const override;

	// �޸ģ�����Ability����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<UGameplayAbility>> MyAbilities;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
