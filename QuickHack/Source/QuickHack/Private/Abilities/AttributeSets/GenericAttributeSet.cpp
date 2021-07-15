// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeSets/GenericAttributeSet.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Characters/CharacterBase.h"

UGenericAttributeSet::UGenericAttributeSet()
: Health(100) ,
MaxHealth(100),
CyberdeckRAM(8),
MaxCyberdeckRAM(8)
{
	
}

void UGenericAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	// TODO Refactor Findfield Checked
	if(Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UGenericAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGenericAttributeSet, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetBaseValue()));
		OnHealthChanged.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
	
	if(Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UGenericAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGenericAttributeSet, CyberdeckRAM)))
	{
		CyberdeckRAM.SetCurrentValue(FMath::Clamp(CyberdeckRAM.GetCurrentValue(), 0.0f, MaxCyberdeckRAM.GetCurrentValue()));

		ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwningActor());
		if(CyberdeckRAM.GetCurrentValue() == MaxCyberdeckRAM.GetCurrentValue())
		{
			if(CharacterBase)
			{
				CharacterBase->AddGameplayTag(CharacterBase->FullRAMTag);
			}
		}
		else
		{
			if(CharacterBase)
			{
				CharacterBase->RemoveGameplayTag(CharacterBase->FullRAMTag);
			}
		}
		
		float Ratio = CyberdeckRAM.GetCurrentValue() / MaxCyberdeckRAM.GetCurrentValue();
		OnCyberdeckRAMChanged.Broadcast(CyberdeckRAM.GetCurrentValue(), MaxCyberdeckRAM.GetCurrentValue(), Ratio);
	}
	
	if(Data.EvaluatedData.Attribute.GetUProperty() ==
		FindFieldChecked<FProperty>(UGenericAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGenericAttributeSet, StartupDelay)))
	{
		OnStartupDelayChanged.Broadcast(StartupDelay.GetCurrentValue(), MaxStartupDelay.GetCurrentValue());
	}
}

void UGenericAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenericAttributeSet, Health, OldHealth);
}


void UGenericAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenericAttributeSet, MaxHealth, OldHealth);
}

void UGenericAttributeSet::OnRep_StartupDelay(const FGameplayAttributeData& OldStartupDelay)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenericAttributeSet, StartupDelay, OldStartupDelay);
}

void UGenericAttributeSet::OnRep_MaxStartupDelay(const FGameplayAttributeData& OldMaxStartupDelay)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenericAttributeSet, MaxStartupDelay, OldMaxStartupDelay);
}


void UGenericAttributeSet::OnRep_CyberdeckRAM(const FGameplayAttributeData& OldCyberdeckRAM)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenericAttributeSet, CyberdeckRAM, OldCyberdeckRAM);
}

void UGenericAttributeSet::OnRep_MaxCyberdeckRAM(const FGameplayAttributeData& OldMaxCyberdeckRAM)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenericAttributeSet, CyberdeckRAM, OldMaxCyberdeckRAM);
}



void UGenericAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UGenericAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGenericAttributeSet, Health, COND_None, REPNOTIFY_Always);
}



