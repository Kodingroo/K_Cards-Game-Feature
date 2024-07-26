// Copyright Isaac Hayward

#include "K_CardsSelectionManagerComponent.h"
#include "Card/K_CardState.h"

UK_CardsSelectionManagerComponent::UK_CardsSelectionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UK_CardsSelectionManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UK_CardsSelectionManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UK_CardsSelectionManagerComponent::SelectCard(UCardObject* Card)
{
	if (IsValid(Card) && !SelectedCards.Contains(Card) && Card != GetLastSelectedCard())
	{
		SelectedCards.Add(Card);
		UpdateCardState(Card, ECardState::Selected);
		// PrintSelectedCards();
	}
}

void UK_CardsSelectionManagerComponent::DeselectCard(UCardObject* Card)
{
	if (IsValid(Card) && SelectedCards.Contains(Card))
	{
		SelectedCards.Remove(Card);
		UpdateCardState(Card, ECardState::Passive);
		// PrintSelectedCards();
	}
}

void UK_CardsSelectionManagerComponent::ClearSelection()
{
	for (UCardObject* Card : SelectedCards)
	{
		if (IsValid(Card))
		{
			UpdateCardState(Card, ECardState::Passive);
		}
	}
	SelectedCards.Empty();
}

TArray<UCardObject*> UK_CardsSelectionManagerComponent::GetSelectedCards() const
{
	return SelectedCards;
}

UCardObject* UK_CardsSelectionManagerComponent::GetLastSelectedCard() const
{
	if (SelectedCards.Num() > 0)
	{
		return SelectedCards.Last();
	}
	return nullptr;
}

void UK_CardsSelectionManagerComponent::PrintSelectedCards() const
{
	UE_LOG(LogTemp, Warning, TEXT("Selected Cards:"));
	for (int32 i = 0; i < SelectedCards.Num(); ++i)
	{
		if (SelectedCards[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("[%d]: %s"), i, *SelectedCards[i]->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%d]: nullptr"), i);
		}
	}
}

void UK_CardsSelectionManagerComponent::UpdateCardState(UCardObject* Card, ECardState NewState)
{
	if (IsValid(Card))
	{
		Card->CardState = NewState;
	}
}
