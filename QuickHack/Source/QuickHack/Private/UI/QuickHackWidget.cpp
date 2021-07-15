// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QuickHackWidget.h"
#include "Components/HorizontalBox.h"


void UQuickHackWidget::HandleRAMOnPanelHover(int32 RAMCost)
{
	if(ChildrenLength <= 0)
	{
		return;
	}
	
	RAMRevCounter = 0;
	
	for(int i = ChildrenLength; i > 0; i--)
	{
		RAMRevCounter++;
		if(RAMRevCounter <= RAMCost)
		{
			OnRAMSelected(i);
		}
		else
		{
			OnRAMDeselected(i);
		}
	}
	
	for(int i = RAMHorizontalBox->GetChildrenCount(); i >= RAMHorizontalBox->GetChildrenCount() - UsedChildrenLength; i--)
	{
		OnRAMUsed(i);
	}
	
}


void UQuickHackWidget::SetChidlrenLengthBasedOnCurrentRAM(int32 CurrentRAM)
{
	ChildrenLength = CurrentRAM;
}


void UQuickHackWidget::SetUsedChildren(int32 CurrentRAM, int32 MaxRAM)
{
	UsedChildrenLength = MaxRAM - CurrentRAM;
}