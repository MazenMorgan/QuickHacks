// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GenericAbilitySystemComponent.h"

#include "Abilities/GenericGameplayAbility.h"

void UGenericAbilitySystemComponent::GetActiveAbilityOnActorWithTag()
{
	FGameplayAbilitySpecContainer AbilitiesSpec = UAbilitySystemComponent::ActivatableAbilities;
	
	ABILITYLIST_SCOPE_LOCK();
	for(FGameplayAbilitySpec AbilitySpec : AbilitiesSpec.Items)
	{
		UGenericGameplayAbility* GenericAbility = Cast<UGenericGameplayAbility>(AbilitySpec.Ability);
		if(GenericAbility)
		{
			//TODO Get the time remaining.
		}
	}
}
