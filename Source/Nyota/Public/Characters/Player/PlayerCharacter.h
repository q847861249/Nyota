// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NYOTA_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();
private:
	// Camera components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
protected:
	virtual void BeginPlay() override;
	
	/** Replicated function called when the player is possessed by a controller (server side)*/ 
	virtual void PossessedBy(AController* NewController) override;
	/** Replicated function called when the player state is replicated to clients */
	virtual void OnRep_PlayerState() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**When the Capsule componet trigger overlap event, this function will be called to check overlap actor and send event */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
