// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/GenericAbilitySystemComponent.h"
#include "Abilities/AttributeSets/GenericAttributeSet.h"
#include "Abilities/GenericGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Framework/PlayerControllerBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UGenericAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	Attributes = CreateDefaultSubobject<UGenericAttributeSet>(TEXT("Attributes"));

	Attributes->OnHealthChanged.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	Attributes->OnStartupDelayChanged.AddDynamic(this, &ACharacterBase::OnStartupDelayChanged);
	Attributes->OnCyberdeckRAMChanged.AddDynamic(this, &ACharacterBase::OnCyberdeckRAMChanged);
}

void ACharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ACharacterBase::BeginPlay"));
	InitializeTeamID();
	GiveAbilities();
}


void ACharacterBase::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Character Died"));
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		DisableInput(PlayerController);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		//AIC->GetBrainComponent()->StopLogic("Dead");
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::InitializeTeamID()
{
	if(GetController() && GetController()->IsPlayerController())
	{
		TeamID = 0;
	}
	else
	{
		TeamID = 1;
	}
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other)
{
	if(Other)
	{
		return this->TeamID != Other->TeamID;
	}
	return false;
}


void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	if(Health <= 0.0f)
	{
		Die();
	}
	BP_OnHealthChanged(Health, MaxHealth);
}


void ACharacterBase::OnStartupDelayChanged(float StartupDelay, float MaxStartupDelay)
{
	BP_OnStartupDelayChanged(StartupDelay, MaxStartupDelay);
}


void ACharacterBase::OnCyberdeckRAMChanged(float CyberdeckRAM, float MaxCyberdeckRAM, float Ratio)
{
	BP_OnCyberdeckRAMChanged(CyberdeckRAM, MaxCyberdeckRAM, Ratio);
}


#pragma region GameplayAbilityStuff
UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	//GiveAbilities();
}


void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EQHAbilityInputID" ,
			static_cast<int32>(EQHAbilityInputID::Confirm), static_cast<int32>(EQHAbilityInputID::Confirm));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}


void ACharacterBase::InitializeAttributes()
{
	if(AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		// Effects can have different levels. so we create a spec
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContextHandle);
		
		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}


void ACharacterBase::GiveAbilities()
{
	UE_LOG(LogTemp, Warning, TEXT("Giving Ability"));
	if(HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGenericGameplayAbility>& AbilityClass : DefaultAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, static_cast<int32>(AbilityClass.GetDefaultObject()->QHAbilityInputID), this);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
			AddAbilityToUI(*AbilityClass);
		}
	}
}


void ACharacterBase::AddAbilityToUI(TSubclassOf<UGenericGameplayAbility> AbilityToAdd)
{
	APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(GetController());

	if(PlayerControllerBase && AbilityToAdd)
	{
		UGenericGameplayAbility* AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGenericGameplayAbility>();
		if(AbilityInstance)
		{
			FGameplayQuickHackAbilityInfo AbilityInfo = AbilityInstance->GetQuickHackAbilityInfo();
			AbilityInfosMap.Add(AbilityInfo.AbilityClass, AbilityInfo);
			BP_AddAbilityToUI(AbilityInfo);
		}
	}
}


void ACharacterBase::AquireAbility(TSubclassOf<UGenericGameplayAbility> AbilityToAquire)
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityToAquire, 1, static_cast<int32>(AbilityToAquire.GetDefaultObject()->QHAbilityInputID), this);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}


void ACharacterBase::AddGameplayTag(FGameplayTag& TagToAdd)
{
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
		AbilitySystemComponent->TagContainer.AddTag(TagToAdd);
	}
}  


void ACharacterBase::RemoveGameplayTag(FGameplayTag& TagToRemove)
{
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
		AbilitySystemComponent->TagContainer.RemoveTag(TagToRemove);
	}
}


bool ACharacterBase::HasGameplayTag(FGameplayTag& Tag)
{
	return AbilitySystemComponent->TagContainer.HasTag(Tag);
}


#pragma endregion
