// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Abilities/AbilityTypes.h"
#include "CharacterBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagAquiredOrRemoved, bool, bTagAquired, FGameplayTag, GameplayTag);

class AAIController;

UCLASS()
class QUICKHACK_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// GAS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	class UGenericAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	class UGenericAttributeSet* Attributes;
	 
public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;

	UPROPERTY(BlueprintAssignable)
	FOnTagAquiredOrRemoved OnTagAquiredOrRemoved;
	virtual void Die();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	

	// GAS
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="CharacterBase|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="CharacterBase|Abilities")
	TArray<TSubclassOf<class UGenericGameplayAbility>> DefaultAbilities;
	
	virtual void InitializeAttributes();
	virtual void GiveAbilities();
	virtual void AddAbilityToUI(TSubclassOf<UGenericGameplayAbility> AbilityToAdd);
	
	UFUNCTION(BlueprintCallable, Category="CharacterBase|Abilities")
	virtual void AquireAbility(TSubclassOf<class UGenericGameplayAbility> AbilityToAquire);

	UFUNCTION(BlueprintCallable, Category="CharacterBase|Abilities")
	void AddGameplayTag(UPARAM(ref)FGameplayTag& TagToAdd);
	UFUNCTION(BlueprintCallable, Category="CharacterBase|Abilities")
	void RemoveGameplayTag(UPARAM(ref)FGameplayTag& TagToRemove);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CharacterBase|Abilities")
	bool HasGameplayTag(UPARAM(ref)FGameplayTag& Tag);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CharacterBase|Abilities")
	FGameplayTag FullRAMTag;	
	
protected:
	uint8 TeamID;
	virtual void InitializeTeamID();

	UFUNCTION(BlueprintCallable, Category="CharacterBase")
	bool IsOtherHostile(ACharacterBase* Other);

	UPROPERTY(BlueprintReadOnly, Category="CharacterBase|Abilities")
	TMap<TSubclassOf<class UGenericGameplayAbility>, FGameplayQuickHackAbilityInfo> AbilityInfosMap;
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase", meta = (DisplayName = "AddAbilityToUI"))
	void BP_AddAbilityToUI(FGameplayQuickHackAbilityInfo AbilityInfo);
	
	UFUNCTION()
	virtual void OnHealthChanged(float Health, float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase|Abilities", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION()
	virtual void OnStartupDelayChanged(float StartupDelay, float MaxStartupDelay);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase|Abilities", meta = (DisplayName = "OnStartupDelay"))
	void BP_OnStartupDelayChanged(float StartupDelay, float MaxStartupDelay);

	UFUNCTION()
	virtual void OnCyberdeckRAMChanged(float CyberdeckRAM, float MaxCyberdeckRAM, float Ratio);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase|Abilities", meta = (DisplayName = "OnCyberdeckRAMChanged"))
	void BP_OnCyberdeckRAMChanged(float CyberdeckRAM, float MaxCyberdeckRAM, float Ratio);
};


