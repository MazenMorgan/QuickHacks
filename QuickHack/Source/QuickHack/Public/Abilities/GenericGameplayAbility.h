// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/GameplayAbility.h" 
#include "QuickHack/QuickHack.h"
#include "Abilities/AbilityTypes.h"

#include "GenericGameplayAbility.generated.h"


/**
 * 
 */
UCLASS()
class QUICKHACK_API UGenericGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGenericGameplayAbility();

	UFUNCTION(BlueprintCallable, Category="Ability")
	FGameplayQuickHackAbilityInfo GetQuickHackAbilityInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability")
	UMaterialInstance* UIMat;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability")
	class UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability")
	FName QuickHackName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability")
	EQuickHackStatus QuickHackStatus;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability|Input")
	EQHAbilityInputID QHAbilityInputID = EQHAbilityInputID::None;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Ability|Time")
	float DefaultStartupDelay;

	UFUNCTION(BlueprintCallable, Category="Ability")
	void EndSpecifiedTask(UGameplayTask* TaskToEnd);

	UFUNCTION(BlueprintCallable, Category="Ability")
	void EndSpecifiedTasks(TArray<UGameplayTask*> TasksToEnd);


	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

};
 