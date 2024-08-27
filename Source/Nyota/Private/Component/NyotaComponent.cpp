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

	InitializeWeapon();


}

void UNyotaComponent::SetupOwner()
{
	if (!GetOwner())
	{
		return;
	}


	OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	OwnerPawn = Cast<APawn>(OwnerActor);
	OwnerCharacter = Cast<ACharacter>(OwnerActor);

	OwnerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
}


// Called every frame
void UNyotaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UNyotaComponent::ActivateAbilityByClass(const TSubclassOf<UGameplayAbility> AbilityClass, UGA_Base*& ActivatedAbility, const bool bAllowRemoteActivation )
{
	if (!OwnerAbilitySystemComponent || !AbilityClass)
	{
		return false;
	}

	const bool bSuccess = OwnerAbilitySystemComponent->TryActivateAbilityByClass(AbilityClass, bAllowRemoteActivation);

	TArray<UGameplayAbility*> ActiveAbilities = GetActiveAbilitiesByClass(AbilityClass);
	if (ActiveAbilities.Num() == 0)
	{
		Debug::SLOG(TEXT("UGSCCoreComponent::ActivateAbilityByClass Couldn't get back active abilities with Class %s. Won't be able to return ActivatedAbility instance."));

	}

	if (bSuccess && ActiveAbilities.Num() > 0)
	{
		UGA_Base* Ability = Cast<UGA_Base>(ActiveAbilities[0]);
		if (Ability)
		{
			ActivatedAbility = Ability;
		}
	}

	return bSuccess;
}

TArray<UGameplayAbility*> UNyotaComponent::GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> AbilityToSearch) const
{
	if (!OwnerAbilitySystemComponent)
	{
		Debug::SLOG(TEXT("UGSCCoreComponent::GetActiveAbilitiesByClass() ASC is not valid"));
		return {};
	}

	TArray<FGameplayAbilitySpec> Specs = OwnerAbilitySystemComponent->GetActivatableAbilities();
	TArray<struct FGameplayAbilitySpec*> MatchingGameplayAbilities;
	TArray<UGameplayAbility*> ActiveAbilities;

	// First, search for matching Abilities for this class
	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		if (Spec.Ability && Spec.Ability->GetClass()->IsChildOf(AbilityToSearch))
		{
			MatchingGameplayAbilities.Add(const_cast<FGameplayAbilitySpec*>(&Spec));
		}
	}

	// Iterate the list of all ability specs
	for (const FGameplayAbilitySpec* Spec : MatchingGameplayAbilities)
	{
		// Iterate all instances on this ability spec, which can include instance per execution abilities
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			if (ActiveAbility->IsActive())
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}

	return ActiveAbilities;
}


UAnimMontage* UNyotaComponent::GetCurrentCombSequence()
{
	if (ANyotaCharacters* character = Cast<ANyotaCharacters>(GetOwner()))
	{
		float CurrentMontageIndex = character->GetAbilitySystemComponent()->GetNumericAttributeBase(UNyotaAttributeSet::GetComboIndexAttribute());

		if (!(CurrentMontageIndex >= 0))
		{
			if (GetOwner()->HasAuthority()) Debug::SLOG(FString::Printf(TEXT("Server: %s doesn't have Animation to play"), *CurrentWeapon->GetName()));
			if (!GetOwner()->HasAuthority()) Debug::SLOG(FString::Printf(TEXT("Client: %s doesn't have Animation to play"), *CurrentWeapon->GetName()));

			return nullptr;
		}

		return CurrentWeapon->AttackAnimMontage[CurrentMontageIndex];
	}
	
	return nullptr;
}

UGameplayAbility* UNyotaComponent::GetCurrentActiveComboAbility() const
{
	TArray<UGameplayAbility*> Abilities = GetActiveAbilitiesByClass(MeleeBaseAbility);
	return Abilities.IsValidIndex(0) ? Abilities[0] : nullptr;
}

void UNyotaComponent::ResetCombo()
{
	SetComboIndex(0);
}

void UNyotaComponent::IncrementCombo()
{
	if (bComboWindowOpened)
	{
		ComboIndex = ComboIndex + 1;
	}

}

void UNyotaComponent::SetComboIndex(int32 InComboIndex)
{
	if (!IsNetSimulating())
	{
		ComboIndex = InComboIndex;
	}
	else
	{


		ComboIndex = InComboIndex;
		ServerSetComboIndex(InComboIndex);
	}

}

void UNyotaComponent::ServerSetComboIndex_Implementation(int32 InComboIndex)
{
	MulticastSetComboIndex(InComboIndex);
}

void UNyotaComponent::MulticastSetComboIndex_Implementation(int32 InComboIndex)
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter && !OwnerCharacter->IsLocallyControlled())
	{
		ComboIndex = InComboIndex;
	}
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

	return CurrentWeapon->IsValidLowLevelFast();
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

					if (CurrentMontageIndex + 1 < CurrentWeapon->AttackAnimMontage.Num())
					{
						character->GetAbilitySystemComponent()->SetNumericAttributeBase(UNyotaAttributeSet::GetComboIndexAttribute(), CurrentMontageIndex + 1);
					}

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
					if ((CurrentMontageIndex + 1) == CurrentWeapon->AttackAnimMontage.Num())
					{
						character->GetAbilitySystemComponent()->SetNumericAttributeBase(UNyotaAttributeSet::GetComboIndexAttribute(),0);
					}
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
