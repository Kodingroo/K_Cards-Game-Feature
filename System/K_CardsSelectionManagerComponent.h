// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Card/K_CardObject.h"
#include "K_CardsSelectionManagerComponent.generated.h"

/**
 * Manages the selection and deselection of card objects.
 * Provides functionalities to select, deselect, and get selected cards.
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_CARDSRUNTIME_API UK_CardsSelectionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UK_CardsSelectionManagerComponent();
	
	void SelectCard(UCardObject* Card);
	void DeselectCard(UCardObject* Card);

	// Clears all selected cards
	UFUNCTION(BlueprintCallable, Category = "Card Selection")
	void ClearSelection();

	// Returns the list of currently selected cards
	UFUNCTION(BlueprintCallable, Category = "Card Selection")
	TArray<UCardObject*> GetSelectedCards() const;

	UFUNCTION(BlueprintCallable, Category = "Card Selection")
	UCardObject* GetLastSelectedCard() const;

	void PrintSelectedCards() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// List of currently selected cards
	UPROPERTY(VisibleAnywhere, Category = "Card Selection")
	TArray<TObjectPtr<UCardObject>> SelectedCards;

	// Helper function to update card states
	void UpdateCardState(UCardObject* Card, ECardState NewState);
};
