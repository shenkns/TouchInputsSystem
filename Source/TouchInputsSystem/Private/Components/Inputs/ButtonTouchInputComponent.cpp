// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Components/Inputs/ButtonTouchInputComponent.h"

#include "LogSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/ButtonTouchInputDebugWidget.h"
#include "LogSystem.h"
#include "Module/TouchInputsSystemModule.h"
#include "Module/TouchInputsSystemSettings.h"
#include "TouchInputsConfigurationObjects/ButtonTouchInputSaveObject.h"

UButtonTouchInputComponent::UButtonTouchInputComponent()
{
	DebugWidgetClass = UButtonTouchInputDebugWidget::StaticClass();

	MaxFingersCount = 2;

	DoubleTapInterval = 0.25;
	TwoFingersTapInterval = 0.1;

	Tint = FLinearColor::White;
}

void UButtonTouchInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UButtonTouchInputComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	const UTouchInputsSystemSettings* Settings = GetDefault<UTouchInputsSystemSettings>();
	if(!Settings || !Settings->bShowDebugShapes) return;
	if(!DebugWidget) return;

	DebugWidget->SetVisibility(ESlateVisibility::Visible);
}

void UButtonTouchInputComponent::Deactivate()
{
	Super::Deactivate();

	if(!DebugWidget) return;

	DebugWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UButtonTouchInputComponent::OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchPressed(FingerIndex, Location);

	if(Fingers.Num() == MaxFingersCount)
	{
		if((FDateTime::UtcNow() - LastTapTime).GetTotalSeconds() < TwoFingersTapInterval)
		{
			if((FDateTime::UtcNow() - TwoFingersTapTime).GetTotalSeconds() < DoubleTapInterval)
			{
				TwoFingersTapTime = FDateTime::MinValue();

				OnTwoFingersDoubleTap.Broadcast(GetFingerLocationAtIndex(0), GetFingerIndexAtIndex(0), GetFingerLocationAtIndex(1), GetFingerIndexAtIndex(1), (FDateTime::UtcNow() - LastTapTime).GetTotalSeconds());

				LOG(LogTouchInputsSystem, "Two Fingers Double Tap: Touch %d %s Touch %d %s, Tap Time: %d ms",
					GetFingerIndexAtIndex(0).GetValue(),
					*GetFingerLocationAtIndex(0).ToString(),
					GetFingerIndexAtIndex(1).GetValue(),
					*GetFingerLocationAtIndex(1).ToString(),
					(int)(FDateTime::UtcNow() - LastTapTime).GetTotalSeconds())
				
				if(ValidateDebugWidget()) Cast<UButtonTouchInputDebugWidget>(DebugWidget)->bDoubleTap = true;
			}
			else
			{
				TwoFingersTapTime = FDateTime::UtcNow();

				OnTwoFingersTap.Broadcast(GetFingerLocationAtIndex(0), GetFingerIndexAtIndex(0), GetFingerLocationAtIndex(1), GetFingerIndexAtIndex(1));

				LOG(LogTouchInputsSystem, "Two Fingers Tap: Touch %d %s Touch %d %s",
					GetFingerIndexAtIndex(0).GetValue(),
					*GetFingerLocationAtIndex(0).ToString(),
					GetFingerIndexAtIndex(1).GetValue(),
					*GetFingerLocationAtIndex(1).ToString())
			}

			if(ValidateDebugWidget())
			{
				Cast<UButtonTouchInputDebugWidget>(DebugWidget)->TapLocations = { FVector2D(GetFingerLocationAtIndex(0)), FVector2D(GetFingerLocationAtIndex(1)) };
				DebugWidget->BoundsSize = BoundsSize;
				DebugWidget->BoundsOrigin = BoundsOrigin;
			}
		}
		else
		{
			OnTwoFingersTapFailed.Broadcast((FDateTime::UtcNow() - TwoFingersTapTime).GetTotalSeconds());

			LOG(LogTouchInputsSystem, "Two Fingers Tap Failed, Tap Time: %d ms", (int)(FDateTime::UtcNow() - TwoFingersTapTime).GetTotalSeconds())
		}
	}
	else
	{
		LastTapTime = FDateTime::UtcNow();

		if((LastTapTime - OneFingerTapTime).GetTotalSeconds() < DoubleTapInterval)
		{
			OneFingerTapTime = FDateTime::MinValue();

			OnDoubleTap.Broadcast(Location, FingerIndex, (LastTapTime - OneFingerTapTime).GetTotalSeconds());

			LOG(LogTouchInputsSystem, "Double Tap: Touch %d %s, Tap Time: %d ms",
				FingerIndex,
				*Location.ToString(),
				(int)(LastTapTime - OneFingerTapTime).GetTotalSeconds())

			if(ValidateDebugWidget()) Cast<UButtonTouchInputDebugWidget>(DebugWidget)->bDoubleTap = true;
		}
		else
		{
			OneFingerTapTime = LastTapTime;

			OnTap.Broadcast(Location, FingerIndex);

			LOG(LogTouchInputsSystem, "Tap: Touch %d %s", FingerIndex, *Location.ToString())
		}

		if(ValidateDebugWidget() && Cast<UButtonTouchInputDebugWidget>(DebugWidget)) Cast<UButtonTouchInputDebugWidget>(DebugWidget)->TapLocations = { FVector2D(Location) };
	}

	if(ValidateDebugWidget())
	{
		DebugWidget->BoundsSize = BoundsSize;
		DebugWidget->BoundsOrigin = BoundsOrigin;
	}
}

void UButtonTouchInputComponent::OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchMoved(FingerIndex, Location);

	OnTapMove.Broadcast(Location, FingerIndex);

	const UTouchInputsSystemSettings* Settings = GetDefault<UTouchInputsSystemSettings>();
	if(Settings && Settings->bShowDebugShapes && DebugWidget)
	{
		DebugWidget->BoundsSize = BoundsSize;
		DebugWidget->BoundsOrigin = BoundsOrigin;
	}
}

void UButtonTouchInputComponent::OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	Super::OnEventTouchReleased(FingerIndex, Location);

	OnTapRelease.Broadcast(Location, FingerIndex);

	LOG(LogTouchInputsSystem, "Release: Touch %d %s", FingerIndex, *Location.ToString())

	const UTouchInputsSystemSettings* Settings = GetDefault<UTouchInputsSystemSettings>();
	if(Settings && Settings->bShowDebugShapes && DebugWidget)
	{
		DebugWidget->RemoveFromParent();
		DebugWidget = nullptr;
	}
}

void UButtonTouchInputComponent::SetupBounds()
{
	Super::SetupBounds();

	SetupBackgroundsByBounds(BackgroundTexture, Tint);
}

bool UButtonTouchInputComponent::ValidateDebugWidget()
{
	if(Super::ValidateDebugWidget())
	{
		ButtonDebugWidget = Cast<UButtonTouchInputDebugWidget>(DebugWidget);

		return IsValid(ButtonDebugWidget);
	}

	return false;
}

UTouchInputSaveObject* UButtonTouchInputComponent::CreateSaveObject()
{
	return NewObject<UButtonTouchInputSaveObject>(this);
}

void UButtonTouchInputComponent::AddSaveDataToObject(UTouchInputSaveObject* SaveObject)
{
	Super::AddSaveDataToObject(SaveObject);

	if(!IsValid(SaveObject)) return;

	if(UButtonTouchInputSaveObject* ButtonSaveObject = Cast<UButtonTouchInputSaveObject>(SaveObject))
	{
		ButtonSaveObject->Tint = Tint;
		ButtonSaveObject->Background = BackgroundTexture;
	}
}

void UButtonTouchInputComponent::LoadDataFromSaveObject(UTouchInputSaveObject* SaveObject)
{
    if(!IsValid(SaveObject)) return;
    
	Super::LoadDataFromSaveObject(SaveObject);

	if(const UButtonTouchInputSaveObject* ButtonSaveObject = Cast<UButtonTouchInputSaveObject>(SaveObject))
	{
		Tint = ButtonSaveObject->Tint;
	}
}
