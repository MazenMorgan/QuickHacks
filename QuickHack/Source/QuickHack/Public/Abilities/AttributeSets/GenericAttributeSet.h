// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GenericAttributeSet.generated.h"

// This code is located inside UAttriuteSet as a comment to help the user to create getter and setter faster.
// instead of typing each line
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQHStartupDelayChangedDelegate, float, StartupDelay, float, MaxStartupDelay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCyberdeckRAMChangeDelegate, float, CyberdeckRAM, float, MaxCyberdeckRAM, float, Ratio);
/**
 * 
 */
UCLASS()
class QUICKHACK_API UGenericAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGenericAttributeSet();
 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FOnHealthChangedDelegate OnHealthChanged;
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FOnQHStartupDelayChangedDelegate OnStartupDelayChanged;
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FOnCyberdeckRAMChangeDelegate OnCyberdeckRAMChanged;
	
#pragma region HealthAttribute
	// Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGenericAttributeSet, Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
#pragma endregion

#pragma region MaxHealthAttribute
	// Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGenericAttributeSet, MaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldHealth);
#pragma endregion

#pragma region StartupDelay
	// Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_StartupDelay)
	FGameplayAttributeData StartupDelay;
	ATTRIBUTE_ACCESSORS(UGenericAttributeSet, StartupDelay);
	UFUNCTION()
	virtual void OnRep_StartupDelay(const FGameplayAttributeData& OldStartupDelay);
#pragma endregion

#pragma region MaxStartupDelay
	// Attributes
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MaxStartupDelay)
	FGameplayAttributeData MaxStartupDelay;
	ATTRIBUTE_ACCESSORS(UGenericAttributeSet, MaxStartupDelay);
	UFUNCTION()
	virtual void OnRep_MaxStartupDelay(const FGameplayAttributeData& OldMaxStartupDelay);
#pragma endregion 

#pragma region CyberdeckRAM
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_CyberdeckRAM)
	FGameplayAttributeData CyberdeckRAM;
	ATTRIBUTE_ACCESSORS(UGenericAttributeSet, CyberdeckRAM);
	UFUNCTION()
	virtual void OnRep_CyberdeckRAM(const FGameplayAttributeData& OldCyberdeckRAM);
#pragma endregion

#pragma region MaxCyberdeckRAM
	UPROPERTY(BlueprintReadOnly, Category="Attributes", ReplicatedUsing=OnRep_MaxCyberdeckRAM)
	FGameplayAttributeData MaxCyberdeckRAM;
	ATTRIBUTE_ACCESSORS(UGenericAttributeSet, MaxCyberdeckRAM);
	UFUNCTION()
	virtual void OnRep_MaxCyberdeckRAM(const FGameplayAttributeData& OldMaxCyberdeckRAM);
#pragma endregion 

};
