// Copyright shenkns Touch Inputs System Developed With Unreal Engine. All Rights Reserved 2022.

#include "Components/Inputs/TouchInputComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/ButtonTouchInputDebugWidget.h"
#include "LogSystem.h"
#include "ManagersSystem.h"
#include "Interfaces/TouchInputWidgetInterface.h"
#include "Managers/StatsManager.h"
#include "Module/TouchInputsSystemModule.h"
#include "Module/TouchInputsSystemSettings.h"
#include "Stats/TouchInputsStat.h"
#include "TouchInputsConfigurationObjects/TouchInputSaveObject.h"

UTouchInputComponent::UTouchInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bAutoActivate = false;

	DebugWidgetClass = UTouchInputDebugWidget::StaticClass();

	Priority = 0;
	
	bUseBounds = true;
	BoundsOriginSetup = FVector2D(0.f, 0.f);
	BoundsSizeSetup = FVector2D(1.f, 1.f);
	bRectangular = true;
	bBoundsPercentage = true;

	bSaveInputData = true;
}

void UTouchInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(UKismetSystemLibrary::IsDedicatedServer(this)) return;
	
	ViewportChangingCheck();
	NetworkConnectionCheck();
}

bool UTouchInputComponent::CheckViewportSizeChanged()
{
	if(!GEngine->GameViewport) return false;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	if(ActualViewportSize != ViewportSize)
	{
		ActualViewportSize = ViewportSize;

		LOG(LogTouchInputsSystem, "New Viewport Size: %s", *ActualViewportSize.ToString())

		return true;
	}

	return false;
}

void UTouchInputComponent::CheckPawnPossessedByPlayer()
{
	if(const APawn* PawnOwner = GetOwner<APawn>())
	{
		bIsOwnerPawnOrController = true;
		
		if(PawnOwner->GetController<APlayerController>())
		{
			bIsPossessed = true;
		}
		else
		{
			bIsPossessed = false;
		}
	}
	else if(const APlayerController* ControllerOwner = GetOwner<APlayerController>())
	{
		bIsOwnerPawnOrController = true;

		bIsPossessed = IsValid(ControllerOwner->GetPawnOrSpectator());
	}
}

bool UTouchInputComponent::IsBoundingEnabled() const
{
	return !(!bUseBounds || BoundsSize.IsZero());
}

bool UTouchInputComponent::CheckBoundsX(FVector Location) const
{
	return !IsBoundingEnabled() || (Location.X > BoundsOrigin.X && Location.X < (BoundsOrigin + BoundsSize).X);
}

bool UTouchInputComponent::CheckBoundsY(FVector Location) const
{
	return !IsBoundingEnabled() || (Location.Y > BoundsOrigin.Y && Location.Y < (BoundsOrigin + BoundsSize).Y);
}

bool UTouchInputComponent::CheckBounds(FVector Location) const
{
	return CheckBoundsX(Location) && CheckBoundsY(Location);
}

void UTouchInputComponent::ViewportChangingCheck()
{
	if(UKismetSystemLibrary::IsDedicatedServer(this)) return;
	TempViewportSize = ActualViewportSize;

	if(CheckViewportSizeChanged())
	{
		UpdateBoundsInPercent(true);

		UpdateBackgroundWidget();
		UpdateViewportSize();

		OnViewportSizeChanged.Broadcast(FVector2D(TempViewportSize.X, TempViewportSize.Y), FVector2D(ActualViewportSize.X, ActualViewportSize.Y));
	}
	else
	{
		UpdateBoundsInPercent(false);
	}
}

void UTouchInputComponent::NetworkConnectionCheck()
{
	if(!bCheckNetworkConnection) return;

	if(!IsPlayerControllerLoaded()) return;

	bCheckNetworkConnection = false;
}

void UTouchInputComponent::OnPawnPossessed(AController* NewController)
{
	bIsPossessed = NewController->IsPlayerController();
	if(!bIsPossessed) return;

	if(BackgroundWidget)
	{
		BackgroundWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTouchInputComponent::OnPawnUnPossessed(AController* OldController)
{
	if(!OldController->IsPlayerController()) return;

	for(const TTuple<TEnumAsByte<ETouchIndex::Type>, FVector>& Finger : Fingers)
	{
		OnEventTouchReleased(Finger.Key, Finger.Value);
	}

	if(IsValid(BackgroundWidget))
	{
		BackgroundWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	bIsPossessed = false;
}

void UTouchInputComponent::OnEventTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
	AddFingerLocation(Location, FingerIndex);
}

void UTouchInputComponent::OnEventTouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
	UpdateFingerLocation(Location, FingerIndex);
}

void UTouchInputComponent::OnEventTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
	RemoveFingerLocation(FingerIndex);
}

bool UTouchInputComponent::AddFingerLocation(FVector Location, TEnumAsByte<ETouchIndex::Type> FingerIndex)
{
	if(Fingers.Contains(FingerIndex))
	{
		return false;
	}
	
	Fingers.Add(FingerIndex, Location);
	return true;
}

void UTouchInputComponent::RemoveFingerLocation(TEnumAsByte<ETouchIndex::Type> FingerIndex)
{
	Fingers.Remove(FingerIndex);
}

void UTouchInputComponent::UpdateFingerLocation(FVector NewLocation, TEnumAsByte<ETouchIndex::Type> FingerIndex)
{
	Fingers.Emplace(FingerIndex, NewLocation);
}

TEnumAsByte<ETouchIndex::Type> UTouchInputComponent::GetFingerIndexAtIndex(int Index) const
{
	TArray<TEnumAsByte<ETouchIndex::Type>> FingerIndexesArray;
	Fingers.GenerateKeyArray(FingerIndexesArray);

	return FingerIndexesArray.IsValidIndex(Index) ? FingerIndexesArray[Index] : TEnumAsByte<ETouchIndex::Type>();
}

FVector UTouchInputComponent::GetFingerLocationAtIndex(int Index) const
{
	TArray<FVector> FingerLocationsArray;
	Fingers.GenerateValueArray(FingerLocationsArray);

	return FingerLocationsArray.IsValidIndex(Index) ? FingerLocationsArray[Index] : FVector();
}

void UTouchInputComponent::UpdateBoundsInPercent(bool bIsViewportChanged)
{
	if(!bIsViewportChanged || !bUseBounds || !bBoundsPercentage) return;

	BoundsOrigin = ActualViewportSize * BoundsOriginPercentage;

	const FVector2D NonRectangularBoundsSize = ActualViewportSize * BoundsSizePercentage;
	
	if(bRectangular)
	{
		const float MaxNonRectangularBoundsSizeAxis = FMath::Max(NonRectangularBoundsSize.X, NonRectangularBoundsSize.Y);
		BoundsSize = FVector2D(MaxNonRectangularBoundsSizeAxis, MaxNonRectangularBoundsSizeAxis);

		LOG(LogTouchInputsSystem, "New Bounds Size: %s", *BoundsSize.ToString())
	}
	else
	{
		BoundsSize = NonRectangularBoundsSize;

		LOG(LogTouchInputsSystem, "New Bounds Size: %s", *BoundsSize.ToString())
	}
}

void UTouchInputComponent::UpdateBackgroundWidget()
{
	if(!BackgroundWidget) return;

	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());;

	if(bUseBackgroundWidgetSizeAndOrigin)
	{
		BackgroundWidget->SetPositionInViewport(BoundsOrigin);
		BackgroundWidget->SetDesiredSizeInViewport(BoundsSize / (ViewportScale == 0.f ? 1.f : ViewportScale));
	}
	else
	{
		BackgroundWidget->SetPositionInViewport(bBackgroundWidgetOriginInPercent ? GetScreenSizeBySizeInPercent(BackgroundWidgetForceOrigin) : BackgroundWidgetForceOrigin);

		const FVector2D NonRectangularSize = bBackgroundWidgetOriginInPercent ? GetScreenSizeBySizeInPercent(BackgroundWidgetForceSize) : BackgroundWidgetForceSize;
		if(bBackgroundWidgetRectangularSize)
		{
			const float MaxNonRectangularBoundsSizeAxis = FMath::Max(NonRectangularSize.X, NonRectangularSize.Y);

			BackgroundWidget->SetDesiredSizeInViewport(FVector2D(MaxNonRectangularBoundsSizeAxis, MaxNonRectangularBoundsSizeAxis) / (ViewportScale == 0.f ? 1.f : ViewportScale));
		}
		else
		{
			BackgroundWidget->SetDesiredSizeInViewport(NonRectangularSize);
		}
	}

	LOG(LogTouchInputsSystem, "%s Background Widget Transform Updated", *BackgroundWidget->GetName())
}

FVector2D UTouchInputComponent::GetScreenSizeBySizeInPercent(FVector2D PercentSize) const
{
	return FVector2D(ActualViewportSize.X * PercentSize.X, ActualViewportSize.Y * PercentSize.Y);
}

bool UTouchInputComponent::IsPlayerControllerLoaded() const
{
	if(const APawn* PawnOwner = GetOwner<APawn>())
	{
		return IsValid(PawnOwner->GetController<APlayerController>());
	}
	else if(const APlayerController* ControllerOwner = GetOwner<APlayerController>())
	{
		return IsValid(ControllerOwner);
	}

	return false;
}

bool UTouchInputComponent::IsFingerExists(TEnumAsByte<ETouchIndex::Type> FingerIndex) const
{
	return Fingers.Contains(FingerIndex);
}

void UTouchInputComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	if(BackgroundWidget)
	{
		BackgroundWidget->SetVisibility(ESlateVisibility::Visible);
	}

	LOG(LogTouchInputsSystem, "Activated")
}

void UTouchInputComponent::Deactivate()
{
	Super::Deactivate();

	if(BackgroundWidget)
	{
		BackgroundWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	LOG(LogTouchInputsSystem, "Deactivated")
}

void UTouchInputComponent::SetupBounds()
{
	if(!bUseBounds) return;

	if(bBoundsPercentage)
	{
		BoundsOriginPercentage = BoundsOriginSetup;
		BoundsSizePercentage = BoundsSizeSetup;

		UpdateBoundsInPercent(true);
	}
	else
	{
		BoundsOrigin = BoundsOriginSetup;
		BoundsSize = BoundsSizeSetup;
	}

	LOG(LogTouchInputsSystem, "New Bounds Origin: %s, New Bounds Size: %s", *BoundsOrigin.ToString(), *BoundsSize.ToString())
}

void UTouchInputComponent::SetupBackgroundsByBounds(UTexture2D* Background, FLinearColor Tint)
{
	SetupBackgroundWidget(Background, Tint, true, FVector2D() , false, FVector2D(), false, false);
}

void UTouchInputComponent::SetupBackgroundWidget(UTexture2D* Background, FLinearColor Tint, bool bUseBoundsSizeAndOrigin, FVector2D Origin, bool bOriginInPercent, FVector2D Size, bool bSizeInPercent, bool bIsRectangular)
{
	bUseBackgroundWidgetSizeAndOrigin = bUseBoundsSizeAndOrigin;
	
	BackgroundWidgetForceSize = Size;
	BackgroundWidgetForceOrigin = Origin;

	bBackgroundWidgetOriginInPercent = bOriginInPercent;
	bBackgroundWidgetRectangularSize = bIsRectangular;
	bBackgroundWidgetSizeInPercent = bSizeInPercent;

	if(IsValid(BackgroundWidget))
	{
		BackgroundWidget->RemoveFromParent();
	}

	//if(!IsValid(Background)) return;

	BackgroundWidget = CreateWidget<UUserWidget>(GetWorld(), BackgroundWidgetClass);
	if(!BackgroundWidget) return;

	BackgroundWidget->SetColorAndOpacity(Tint);

	if(BackgroundWidget->GetClass()->ImplementsInterface(UTouchInputWidgetInterface::StaticClass()))
	{
		ITouchInputWidgetInterface::Execute_SetOwningInputComponent(BackgroundWidget, this);
		ITouchInputWidgetInterface::Execute_SetBackground(BackgroundWidget, Background);
	}
	
	BackgroundWidget->AddToViewport();

	CheckPawnPossessedByPlayer();
	if(bIsOwnerPawnOrController && bIsPossessed)
	{
		BackgroundWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	UpdateBackgroundWidget();

	LOG(LogTouchInputsSystem, "%s Backgrounds Widget Initialized", *BackgroundWidget->GetName())
}

void UTouchInputComponent::BeginDestroy()
{
	Super::BeginDestroy();

	if(BackgroundWidget)
	{
		BackgroundWidget->RemoveFromParent();
	}

	if(DebugWidget)
	{
		DebugWidget->RemoveFromParent();
	}
}

void UTouchInputComponent::Init()
{
	Activate(true);
	
	if(bSaveInputData)
	{
		if(!LoadInputData())
		{
			SaveInputData();
		}

	}
	
	CheckViewportSizeChanged();
	CheckPawnPossessedByPlayer();

	SetupBounds();
}

bool UTouchInputComponent::ValidateDebugWidget()
{
	const UTouchInputsSystemSettings* Settings = GetDefault<UTouchInputsSystemSettings>();
	if(!Settings || !Settings->bShowDebugShapes) return false;

	if(!IsValid(DebugWidget))
	{
		DebugWidget = CreateWidget<UTouchInputDebugWidget>(GetWorld(), DebugWidgetClass);
		if(!DebugWidget) return false;

		DebugWidget->AddToViewport();
	}

	DebugWidget->BoundsOrigin = BoundsOrigin;
	DebugWidget->BoundsSize = BoundsSize;

	LOG(LogTouchInputsSystem, "%s Debug Widget Validated", *DebugWidget->GetName())

	return true;
}

void UTouchInputComponent::DestroyComponent(bool bPromoteChildren)
{
	if(IsValid(BackgroundWidget))
	{
		BackgroundWidget->RemoveFromParent();
	}
	
	Super::DestroyComponent(bPromoteChildren);
}

bool UTouchInputComponent::LoadInputData()
{
	const UManagersSystem* MS = UManagersSystem::GetWithContext(this);
	if(!MS) return false;

	const UStatsManager* SM = MS->GetManager<UStatsManager>();
	if(!SM) return false;

	const UTouchInputsStat* Stat = SM->GetStat<UTouchInputsStat>();
	if(!Stat) return false;

	if(!Stat->HasSlotSave(PresetSlot, Slot)) return false;

	UTouchInputSaveObject* SaveObject = Stat->GetSlotSave<UTouchInputSaveObject>(PresetSlot, Slot);
	if(!SaveObject) return false;
	
	LoadDataFromSaveObject(SaveObject);

	return true;
}

void UTouchInputComponent::SaveInputData()
{
	UTouchInputSaveObject* InputSaveObject = CreateSaveObject();
	if(!InputSaveObject) return;

	AddSaveDataToObject(InputSaveObject);

	const UManagersSystem* MS = UManagersSystem::GetWithContext(this);
	if(!MS) return;

	UStatsManager* SM = MS->GetManager<UStatsManager>();
	if(!SM) return;

	UTouchInputsStat* Stat = SM->GetStat<UTouchInputsStat>();
	if(!Stat) return;

	Stat->SaveToSlot(InputSaveObject, PresetSlot, Slot);

	SM->SaveStats();
}

void UTouchInputComponent::AddSaveDataToObject(UTouchInputSaveObject* SaveObject)
{
	if(!IsValid(SaveObject)) return;
	
	SaveObject->BoundsOriginSetup = BoundsOriginSetup;
	SaveObject->BoundsSizeSetup = BoundsSizeSetup;
	SaveObject->bBoundsPercentage = bBoundsPercentage;
	SaveObject->bRectangular = bRectangular;
	SaveObject->ConfigWidgetZOrder = ConfigWidgetZOrder;
}

UTouchInputSaveObject* UTouchInputComponent::CreateSaveObject()
{
	return NewObject<UTouchInputSaveObject>(this);
}

void UTouchInputComponent::LoadDataFromSaveObject(UTouchInputSaveObject* SaveObject)
{
	if(!IsValid(SaveObject)) return;
	
	BoundsOriginSetup = SaveObject->BoundsOriginSetup;
	BoundsSizeSetup = SaveObject->BoundsSizeSetup;
}