// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/QHEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UQHEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	/** Should be called once as part of project setup to load global data tables and tags */
	UAbilitySystemGlobals::Get().InitGlobalData();
}
