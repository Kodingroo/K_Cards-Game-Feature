#include "K_CardObject.h"
#include "K_3DCard.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBox.h"

// Constructor for UCardObject to set default values
UCardObject::UCardObject()
	: Name(FText::FromString("Card Name")),
	  Description(FText::FromString("Card Description")),
	  Type(ECardType::Action),
	  VisualWidgetClass(nullptr),
	  ToolTips(TArray<FDataTableRowHandle>()),
	  Purpose(FGameplayTagContainer()),
	  PlaySettings(FGameplayTagContainer()),
	  CardTags(FGameplayTagContainer()),
	  InteractionConditions(FGameplayTagContainer()),
	  CurrentPileTag(FGameplayTag::RequestGameplayTag(FName("Card.Pile.None"))),
	  CoreValue(0.0f),
	  DamageValue(0.0f),
	  MitigationValue(0.0f),
	  RestoreValue(0.0f),
	  PowerValue(0.0f),
	  MaxUsesPerTurn(1), // Default to 1 use per turn
	  UsesThisTurn(0),
	  RemainingUses(0),
	  Portrait(nullptr),
	  FrameTint(FLinearColor::White),
	  CardFrame(nullptr),
	  CoreFrame(nullptr),
	  DamageFrame(nullptr),
	  MitigationFrame(nullptr),
	  RestoreFrame(nullptr),
	  PowerFrame(nullptr),
	  CardState(ECardState::Passive),
	  CardActor(nullptr),
	  VisualWidget(nullptr),
	  CardWidget(nullptr),
	  InternalCardData()
{
}

void UCardObject::SetCardActor(A3DCard* InCardActor)
{
	CardActor = InCardActor;
}

void UCardObject::ClearCardActor()
{
	CardActor = nullptr;
}

void UCardObject::PopulateCardData(const FCardData& CardData)
{
	// Copy data from FCardData to this object's properties
	Name = CardData.Name;
	Description = CardData.Description;
	Type = CardData.Type;
	VisualWidgetClass = CardData.VisualWidgetClass;
	ToolTips = CardData.ToolTips;

	Purpose = CardData.Purpose;
	PlaySettings = CardData.PlaySettings;
	CardTags = CardData.CardTags;
	InteractionConditions = CardData.InteractionConditions;
	PostUseEvent = CardData.PostUseEvent;
	EndTurnEvent = CardData.EndTurnEvent;

	CoreValue = CardData.CoreValue;
	DamageValue = CardData.DamageValue;
	MitigationValue = CardData.MitigationValue;
	RestoreValue = CardData.RestoreValue;
	PowerValue = CardData.PowerValue;

	Portrait = CardData.Portrait;
	FrameTint = CardData.FrameTint;
	CardFrame = CardData.CardFrame;
	CoreFrame = CardData.CoreFrame;
	DamageFrame = CardData.DamageFrame;
	MitigationFrame = CardData.MitigationFrame;
	RestoreFrame = CardData.RestoreFrame;
	PowerFrame = CardData.PowerFrame;
	
	// Set the maximum uses per turn if specified in CardData
    MaxUsesPerTurn = CardData.MaxUsesPerTurn;
    UsesThisTurn = 0; // Reset uses for the new card
    RemainingUses = MaxUsesPerTurn; // Set the initial remaining uses

	CardState = ECardState::Passive;

	InternalCardData = CardData;

	InitializeVisualWidget();
}

void UCardObject::UpdateVisualWidget()
{
	if (VisualWidget)
	{
		UK_VisualWidgetClass* Class = Cast<UK_VisualWidgetClass>(VisualWidget);
		Class->UpdateCardWidget(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("VisualWidget is null"));
	}
}

void UCardObject::ToggleCardGlow_Implementation(bool bEnable, FLinearColor GlowColor)
{
    // Toggle glow on the Visual widget
    if (VisualWidget)
    {
        if (UK_VisualWidgetClass* WidgetClass = Cast<UK_VisualWidgetClass>(VisualWidget))
        {
            WidgetClass->ToggleCardGlow(bEnable, GlowColor);
        }
    }
}

void UCardObject::SetCurrentCardPile_Implementation(const FGameplayTag& PileName)
{
	if (!PileName.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid PileName in SetCurrentCardPile"));
		return;
	}

	CurrentPileTag = PileName;
}

void UCardObject::InitializeVisualWidget()
{
	if (VisualWidgetClass)
	{
		if (const UWorld* World = GEngine->GameViewport->GetWorld())
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				VisualWidget = CreateWidget<UCommonUserWidget>(PC, VisualWidgetClass);
				UpdateVisualWidget();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("VisualWidgetClass is null"));
	}
}

void UCardObject::RemoveVisualWidget()
{
	if (VisualWidget)
	{
		VisualWidget->RemoveFromParent();

		// If the widget was added to a Slate parent via TakeWidget(), remove it explicitly
		TSharedPtr<SWidget> SlateWidget = VisualWidget->TakeWidget();
		if (SlateWidget.IsValid())
		{
			TSharedPtr<SWidget> ParentWidget = SlateWidget->GetParentWidget();
			if (ParentWidget.IsValid())
			{
				if (TSharedPtr<SOverlay> Overlay = StaticCastSharedPtr<SOverlay>(ParentWidget))
				{
					Overlay->RemoveSlot(SlateWidget.ToSharedRef());
				}
				else if (TSharedPtr<SBox> Box = StaticCastSharedPtr<SBox>(ParentWidget))
				{
					Box->SetContent(SNullWidget::NullWidget); // Reset the content of the box
				}
				// Add additional container types as needed
				else
				{
					// If we have a parent widget but it's not one of the known types, log a warning
					UE_LOG(LogTemp, Warning, TEXT("Unsupported parent widget type for SlateWidget: %s"), *ParentWidget->GetTypeAsString());
				}
			}
		}

		VisualWidget = nullptr;
	}
}

void UCardObject::RegisterUse()
{
	if (CanUse())
	{
		UsesThisTurn++;
		RemainingUses = MaxUsesPerTurn - UsesThisTurn;
		UpdateVisualWidget();
	}
}

bool UCardObject::CanUse() const
{
	return UsesThisTurn < MaxUsesPerTurn;
}

void UCardObject::ResetUses()
{
	UsesThisTurn = 0;
	RemainingUses = MaxUsesPerTurn;
	UpdateVisualWidget();
}
