// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GenericGameplayAbility.h"

#include "GameplayTask.h"


UGenericGameplayAbility::UGenericGameplayAbility()
{
	
}


FGameplayQuickHackAbilityInfo UGenericGameplayAbility::GetQuickHackAbilityInfo()
{
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* RAMCostEffect = GetCostGameplayEffect();
	
	FGameplayQuickHackAbilityInfo InfoResult;
	InfoResult.UploadTime = DefaultStartupDelay;
	InfoResult.UIMat = UIMat;
	InfoResult.AbilityClass = GetClass();
	InfoResult.QuickHackName = QuickHackName;
	InfoResult.QuickHackStatus = QuickHackStatus;
	InfoResult.AbilityTag = AbilityTags.First();
	InfoResult.Description = Description;
	InfoResult.QHIcon = Icon;
	
	if(ensure(CooldownEffect) && ensure(RAMCostEffect))
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, InfoResult.Cooldown);
		if(RAMCostEffect->Modifiers.Num() > 0)
		{
			FGameplayModifierInfo ModifierInfo = RAMCostEffect->Modifiers[0];
			ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, InfoResult.RAMCost);
		}
	}
	
	return InfoResult;
}


void UGenericGameplayAbility::EndSpecifiedTask(UGameplayTask* TaskToEnd)
{
	if(TaskToEnd)
	{
		TaskToEnd->EndTask();
	}
}


void UGenericGameplayAbility::EndSpecifiedTasks(TArray<UGameplayTask*> TasksToEnd)
{
	for(UGameplayTask* TaskToEnd : TasksToEnd)
	{
		EndSpecifiedTask(TaskToEnd);
	}
}

bool UGenericGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	
	bool bStatisfyTagReq = Super::DoesAbilitySatisfyTagRequirements(AbilitySystemComponent, SourceTags, TargetTags, OptionalRelevantTags);

	if(TargetTags)
	{
		TArray<FGameplayTag> TagsContainer;
		TargetTags->GetGameplayTagArray(TagsContainer);
		for(FGameplayTag TagContainer : TagsContainer)
		{
			FString TagName = TagContainer.GetTagName().ToString();
			UE_LOG(LogTemp, Warning, TEXT("Tag Name: %s"), *TagName);
		}
	}

	return bStatisfyTagReq;
}
