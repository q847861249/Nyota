// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NyotaComponent.h"
#include "Character/NyotaCharacters.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/NyotaShooter.h"
#include "Weapon/NyotaMelee.h"
#include "Debug/Debug.h"
#include "Input/NyotaGameplayTags.h"
#include "Character/NyotaMovementComponent.h"
#include "Ability/NyotaAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Ability/GA/GA_Base.h"
#include "./Ability/GA/GA_Attack.h"
#include "Character/NyotaCharacters.h"


// Sets default values for this component's properties


UNyotaComponent::UNyotaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	MeleeBaseAbility = UGA_Attack::StaticClass();
}

// Called when the game starts
void UNyotaComponent::BeginPlay()
{
	Super::BeginPlay();
}



// Called every frame
void UNyotaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}






bool UNyotaComponent::InitializeWeapon()
{
	if (CharacterConfig->NyotaWeapon.Num() == 0) return false;

	for (auto &weapon : CharacterConfig->NyotaWeapon) {

		if (ANyotaCharacters* Character = Cast<ANyotaCharacters>(GetOwner())) {

			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Instigator = Character;
	
			FRotator SpawnTransformRotation = Character->GetRootComponent()->GetRelativeRotation();

			FVector SpawnTransformLocation = Character->GetMesh()->GetSocketLocation(weapon.SocketName);

			CurrentWeapon = GetWorld()->SpawnActor<ANyotaWeapon>(weapon.NyotaWeapon, SpawnTransformLocation, SpawnTransformRotation, SpawnInfo);

			if (CurrentWeapon) CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weapon.SocketName);
		
			break;
		}
	}
	return IsValid(CurrentWeapon);
}




ANyotaWeapon* UNyotaComponent::GetWeaponInstance()
{
	if (ANyotaShooter* shooter = Cast<ANyotaShooter>(CurrentWeapon)) return shooter;
	else if (ANyotaMelee* melee = Cast<ANyotaMelee>(CurrentWeapon)) return melee;
	else return nullptr;
}



void UNyotaComponent::DoMeleeAttack(TSubclassOf<UGameplayAbility> AttackAbility)
{

	if (ANyotaCharacters* character = Cast<ANyotaCharacters>(GetOwner())) 
	{
		if (!character->IsPlayerControlled()) { return; }

		float CurrentMontageIndex = character->GetAbilitySystemComponent()->GetNumericAttributeBase(UNyotaAttributeSet::GetComboIndexAttribute());

		if (character->CharacterMovementComponent) {
		
			if (character->CharacterMovementComponent->Safe_bInAttack)
			{
				if (character->CharacterMovementComponent->Safe_bComboWindow && character->HasAuthority())
				{
				
					FGameplayAbilitySpec* spec = character->AbilitySystem->FindAbilitySpecFromClass(AttackAbility);

					character->AbilitySystem->CancelAbility(spec->Ability);



					character->AbilitySystem->TryActivateAbilityByClass(AttackAbility);
				}
			}
			else
			{
				if (CurrentMontageIndex == 0)
				{
					character->AbilitySystem->TryActivateAbilityByClass(AttackAbility);
				}
				else
				{

				}
			}
		}
	}
}


void UNyotaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, CharacterConfig, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, CurrentActivateProjectileInfo, COND_None, REPNOTIFY_Always);	


	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, ComboIndex, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, bComboWindowOpened, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, bShouldTriggerCombo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, bRequestTriggerCombo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNyotaComponent, bNextComboAbilityActivated, COND_None, REPNOTIFY_Always);

}
