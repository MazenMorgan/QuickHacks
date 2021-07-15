// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "QuickHack/QuickHack.h"

#include "AbilityTypes.generated.h"

class UGenericGameplayAbility;

USTRUCT(BlueprintType)
struct FGameplayQuickHackAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	TSubclassOf<UGenericGameplayAbility> AbilityClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	float UploadTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	float RAMCost;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	FName QuickHackName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	UMaterialInstance* UIMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	EQuickHackStatus QuickHackStatus;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	FGameplayTag AbilityTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHack")
	class UTexture2D* QHIcon;

	FGameplayQuickHackAbilityInfo(): AbilityClass(nullptr), Cooldown(0), UploadTime(0), RAMCost(0), QuickHackName(""),
	                                  UIMat(nullptr), QuickHackStatus(EQuickHackStatus::None), AbilityTag(), Description(), QHIcon(nullptr)
	{
	}

	FGameplayQuickHackAbilityInfo(TSubclassOf<UGenericGameplayAbility> InAbilityClass, float InCooldown, float InUploadTime, float InRAMCost, UMaterialInstance* InUIMat, FName InQuickHackName, FGameplayTag InGameplayTag, FText InDescription, class UTexture2D* InQHIcon):
		AbilityClass(InAbilityClass), Cooldown(InCooldown), UploadTime(InUploadTime), RAMCost(InRAMCost),
		QuickHackName(InQuickHackName), UIMat(InUIMat), QuickHackStatus(EQuickHackStatus::None), AbilityTag(InGameplayTag), Description(InDescription), QHIcon(InQHIcon)
	{
	}
};