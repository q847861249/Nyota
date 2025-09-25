// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NyotaCharacters.h"

#include "Net/UnrealNetwork.h"
#include "Input/NyotaEnhancedInputComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "Component/NyotaComponent.h"
#include "Input/NyotaGameplayTags.h"
#include "Ability/NyotaAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Math/Color.h"
#include "Debug/Debug.h"

#include "Character/NyotaMovementComponent.h"



ANyotaCharacters::ANyotaCharacters(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UNyotaMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	NyotaPlayerController = Cast<ANyotaPlayerController>(GetController());

	//Actor同步
	bReplicates = true;

	//角色基础信息
	NyotaComponent = CreateDefaultSubobject<UNyotaComponent>(TEXT("NyotaCharacterComponent"));

	// GAS basic setting
	AbilitySystem = CreateDefaultSubobject<UNyota_AbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//GAS attribute
	AttributeSet = CreateDefaultSubobject<UNyotaAttributeSet>(TEXT("AttributeSet"));

	//Notify On Attribute Changed
	//character on dead
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ANyotaCharacters::OnHealthAttributeChanged);
	AbilitySystem->RegisterGameplayTagEvent(FNyotaGameplayTags::Get().State_RagDoll, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ANyotaCharacters::OnRagdollStateChanged);

	//Receive Combo tag 
	AbilitySystem->RegisterGameplayTagEvent(FNyotaGameplayTags::Get().State_RagDoll, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ANyotaCharacters::OnRagdollStateChanged);

	//NyotaComponent->SetNetAddressable();
	NyotaComponent->SetIsReplicated(true);


	CharacterMovementComponent = Cast<UNyotaMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->SetIsReplicated(true);

}



// Called when the game starts or when spawned
void ANyotaCharacters::BeginPlay()
{
	Super::BeginPlay();
}


void ANyotaCharacters::GiveAbility()
{

}

void ANyotaCharacters::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	check(NewController);

	if (AbilitySystem) 
	{
		AbilitySystem->InitAbilityActorInfo(this,this);

		if (!NyotaComponent->CharacterConfig) return;

		if (NyotaComponent->CharacterConfig->CharacterAbilityConfig) 
		{
			NyotaComponent->CharacterConfig->CharacterAbilityConfig->GiveAbilityToComponent(AbilitySystem,0);
		}

		ApplyStartUpEffect();
	}


}

UNyota_AbilitySystemComponent* ANyotaCharacters::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

bool ANyotaCharacters::ApplyGameplayEffectToself(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle inEffectHandle)
{
	if (!Effect.Get()) return false;

	FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(Effect, 1, inEffectHandle);

	if (SpecHandle.IsValid()) {

		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGEHandle.WasSuccessfullyApplied();

	}

	return false;
}

void ANyotaCharacters::ApplyStartUpEffect()
{
	if (GetLocalRole() == ROLE_Authority) {

		if (!AbilitySystem) return;

		FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();

		EffectContext.AddSourceObject(this);

	}
}

void ANyotaCharacters::SendGameEventByTag(FGameplayTag Tag)
{
	FGameplayEventData Payload;
	Payload.Instigator = this;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Tag, Payload);
	

}

bool ANyotaCharacters::TryActiveAbilityByTag(FGameplayTag Tag)
{
	FGameplayTagContainer contatiner;

	contatiner.AddTag(Tag);

	bool ActiveResult = AbilitySystem->TryActivateAbilitiesByTag(contatiner);

	if (ActiveResult) return true;
	else return false;
}

void ANyotaCharacters::EnableRagDoll_Implementation()
{
	Rep_EanbleRagdoll_Multicast();
}


void ANyotaCharacters::Rep_EanbleRagdoll_Multicast_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);

	GetCapsuleComponent()->SetCollisionProfileName("CharacterDead");

	GetMesh()->SetCollisionProfileName("Ragdoll");

	if (APlayerController* PlayerController = Cast<APlayerController> (GetController())) {
		DisableInput(PlayerController);
	}

}

// Called every frame
void ANyotaCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
 
}



void ANyotaCharacters::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0 && Data.OldValue > 0) 
	{
		ANyotaCharacters* OtherCharacter = nullptr;
	
		if (Data.GEModData) 
		{
			const FGameplayEffectContextHandle& EffectContent = Data.GEModData->EffectSpec.GetEffectContext();

			OtherCharacter = Cast<ANyotaCharacters>(EffectContent.GetInstigator());

		}

		FGameplayEventData EventPayload;
		EventPayload.EventTag = FNyotaGameplayTags::Get().State_Dead;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FNyotaGameplayTags::Get().State_Dead, EventPayload);
	}
}

void ANyotaCharacters::OnRagdollStateChanged(const FGameplayTag CallbackTag, int32 NewCount)
{

	if(NewCount > 0) StartRagDoll();
}

void ANyotaCharacters::StartRagDoll()
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();


	if (SkeletalMesh && !SkeletalMesh->IsSimulatingPhysics()) 
	{
		SkeletalMesh->SetCollisionProfileName("Ragdoll");
		SkeletalMesh->SetSimulatePhysics(true);

		SkeletalMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		SkeletalMesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		SkeletalMesh->WakeAllRigidBodies();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}



void ANyotaCharacters::PrintString(const FString& string, FLinearColor TextColor, float duration)
{
	int32 PlayInEditorID = GPlayInEditorID;
	if (this->HasAuthority()) 
	{
		GEngine->AddOnScreenDebugMessage(-1, duration, TextColor.ToFColor(true), FString::Printf(TEXT("Server %d %s: "), PlayInEditorID, *string));
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, duration, TextColor.ToFColor(true), FString::Printf(TEXT("Client %d %s: "), PlayInEditorID, *string));
	}
}



