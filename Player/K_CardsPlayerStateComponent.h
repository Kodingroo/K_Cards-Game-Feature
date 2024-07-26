// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "K_CardsPlayerAttributeSet.h"
#include "Card/K_CardObject.h"
#include "Components/PlayerStateComponent.h"
#include "K_CardsPlayerStateComponent.generated.h"

// Delegate for card transfer events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCardTransferred, UCardObject*, CardObject, FGameplayTag, InitialPileTag, FGameplayTag, NewPileTag);

/**
 * Manages player's deck and attributes.
 * Handles card transfers, deck setup, and attribute initialization.
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class K_CARDSRUNTIME_API UK_CardsPlayerStateComponent : public UPlayerStateComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player Stats", Replicated)
    int32 PlayerID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Stats", Replicated)
    int32 Initiative;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player Deck", ReplicatedUsing=OnRep_PlayerDeck)
    TArray<UCardObject*> PlayerDeck;

    UPROPERTY(EditAnywhere, Category="Starting Attributes")
    TArray<FAttributeDefaults> DefaultStartingAttributeData;
    
    UFUNCTION(BlueprintCallable, Category="Starting Deck")
    void SetupStartingDeckData(UDataTable* DataTable, TSubclassOf<UCardObject> CardClass, int32 HandSize);

    // Adds a card to the player's deck. Ensures replication for multiplayer consistency.
    UFUNCTION(BlueprintCallable, Category="Player Deck")
    void AddCardToDeck(UCardObject* Card);

    // Removes a card from the player's deck.
    UFUNCTION(BlueprintCallable, Category="Player Deck")
    void RemoveCardFromDeck(UCardObject* Card);

    // Clears the player's deck, useful for game resets.
    UFUNCTION(BlueprintCallable, Category="Player Deck")
    void ClearDeck();

    // Setup player deck from a DataTable and deal the first hand of cards to the player.
    UFUNCTION(BlueprintCallable, Category="Player Deck")
    void SetupPlayerDeck(UDataTable* CardDataTable, TSubclassOf<UCardObject> CardClass, int32 InitialHandSize);

    // Transfers a card to a new pile and broadcasts the event
    void TransferCardToPile(UCardObject* Card, FGameplayTag NewPileTag);

    // Delegate broadcast when a card is transferred
    UPROPERTY(BlueprintAssignable, Category = "Player Deck")
    FOnCardTransferred OnCardTransferred;

    /** Pile Management */
    void AddCardToPile(UCardObject* Card, const FGameplayTag& PileTag);
    UCardObject* PopCardFromPile(const FGameplayTag& PileTag);
    UCardObject* RemoveSpecificCardFromPile(UCardObject* Card, const FGameplayTag& PileTag);
    int32 GetCardCountInPile(const FGameplayTag& PileTag) const;
    TArray<UCardObject*>& GetCardsInPile(const FGameplayTag& PileTag);

    // Gets all card piles
    TMap<FGameplayTag, TArray<UCardObject*>> GetAllCardPiles() const;

    // Shuffle the draw pile and move cards from the discard pile if necessary
    UFUNCTION(BlueprintCallable, Category = "Player Deck")
    void ShuffleDrawPile();

    void SetupStartingPlayerDeck();

protected:
    virtual void BeginPlay() override;
    virtual void OnRegister() override;

    // Handles the replication of properties.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Notification when PlayerDeck is replicated
    UFUNCTION()
    void OnRep_PlayerDeck();

    // Notification when PlayerAttributes is replicated
    UFUNCTION()
    void OnRep_PlayerAttributes();

    // Server-side function to handle card transfer
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerTransferCardToPile(UCardObject* Card, FGameplayTag NewPileTag);

private:
    UFUNCTION()
    void InitializeAttributes();

    // Map of card piles
    TMap<FGameplayTag, TArray<UCardObject*>> CardPileMap;

    UPROPERTY()
    UDataTable* StartingDeckDataTable;
    TSubclassOf<UCardObject> StartingCardClass;
    int32 StartingHandSize;
};
