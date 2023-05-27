// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Widgets/TouchInputConfigWidget.h"

#include "ManagersSystem.h"
#include "Managers/StatsManager.h"
#include "Stats/TouchInputsStat.h"

void UTouchInputConfigWidget::InitWithSlot_Implementation(UTouchInputPresetSlot* NewPresetSlot, UTouchInputSlotData* NewSlot)
{
	PresetSlot = NewPresetSlot;
	InputSlotData = NewSlot;

	const UManagersSystem* MS = UManagersSystem::Get(this);
	if(!MS) return;

	const UStatsManager* SM = MS->GetManager<UStatsManager>();
	if(!SM) return;

	const UTouchInputsStat* Stat = SM->GetStat<UTouchInputsStat>();
	if(!Stat) return;

	SaveObject = Stat->GetSlotSave<UTouchInputSaveObject>(PresetSlot, InputSlotData);

	LoadDataFromSlot();
}

void UTouchInputConfigWidget::SaveDataToSlot_Implementation()
{
}

void UTouchInputConfigWidget::LoadDataFromSlot_Implementation()
{
	if(!SaveObject) return;

	SetOrigin(SaveObject->BoundsOriginSetup);
	SetSize(SaveObject->BoundsSizeSetup);
}

FReply UTouchInputConfigWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	BoundsOrigin += InGestureEvent.GetCursorDelta();
	SetPositionInViewport(BoundsOrigin);

	if(SaveObject->bBoundsPercentage)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		
		SaveObject->BoundsOriginSetup = BoundsOrigin / ViewportSize;
	}
	else
	{
		SaveObject->BoundsOriginSetup = BoundsOrigin;
	}
	
	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

void UTouchInputConfigWidget::SetSize(FVector2D NewSize)
{
	if(SaveObject->bBoundsPercentage)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		if(SaveObject->bRectangular)
		{
			SetDesiredSizeInViewport(ViewportSize.Y * NewSize);
		}
		else
		{
			SetDesiredSizeInViewport(NewSize * ViewportSize);
		}
	}
	else
	{
		if(SaveObject->bRectangular)
		{
			SetDesiredSizeInViewport(NewSize);
		}
		else
		{
			SetDesiredSizeInViewport(NewSize);
		}
	}

	SaveObject->BoundsSizeSetup = NewSize;
}

void UTouchInputConfigWidget::SetOrigin(FVector2D NewOrigin)
{
	if(SaveObject->bBoundsPercentage)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		BoundsOrigin = NewOrigin * ViewportSize;
		SetPositionInViewport(BoundsOrigin);
	}
	else
	{
		BoundsOrigin = NewOrigin;
		SetPositionInViewport(BoundsOrigin);
	}

	SaveObject->BoundsOriginSetup = NewOrigin;
}
