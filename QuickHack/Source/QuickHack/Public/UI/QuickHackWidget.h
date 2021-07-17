// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickHackWidget.generated.h"

/**
 * 
 */
UCLASS()
class QUICKHACK_API UQuickHackWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* RAMHorizontalBox;

	
	int32 RAMRevCounter = 0;
	int32 ChildrenLength = 0;
	int32 UsedChildrenLength = 0;

	UFUNCTION(BlueprintCallable)
	void HandleRAMOnPanelHover(int32 RAMCost, int32 CurrentRAM, int32 MaxRAM);
	void SetChidlrenLengthBasedOnCurrentRAM(int32 CurrentRAM);
	void SetUsedChildren(int32 CurrentRAM, int32 MaxRAM);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRAMSelected(int32 Index);
	UFUNCTION(BlueprintImplementableEvent)
	void OnRAMDeselected(int32 Index);
	UFUNCTION(BlueprintImplementableEvent)
	void OnRAMUsed(int32 Index);
	
private:


	
};
