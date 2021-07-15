// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"



UENUM(BlueprintType)
enum class EQHAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel")
};

UENUM(BlueprintType)
enum class EQuickHackStatus : uint8
{
	None,
	Ready,
	Blocked,
	Suspended,
	Recompiling
};