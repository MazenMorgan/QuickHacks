// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GenericAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class QUICKHACK_API UGenericAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void GetActiveAbilityOnActorWithTag();

	UPROPERTY(BlueprintReadOnly, Category="GenericAbilitySystemComp")
	FGameplayTagContainer TagContainer;

	
};
