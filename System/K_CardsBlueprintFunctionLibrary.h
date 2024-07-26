// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "K_CardsTurnManagerComponent.h"
#include "Card/K_CardState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "K_CardsBlueprintFunctionLibrary.generated.h"

class UK_CardsTurnManagerComponent;
class UK_CardsSelectionManagerComponent;
class AK_CardsAIController;
class A3DCard;
class UCardObject;
class UK_CardsPlayerAttributeSet;
class UK_CardManager;

/**
 * Provides utility functions for managing cards, player attributes, and AI controllers.
 * Contains functions to interact with gameplay systems and manage card game mechanics.
 */

UCLASS()
class K_CARDSRUNTIME_API UK_CardsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Card Selection Management */
    
    // Gets the Cards Selection Manager
    UFUNCTION(BlueprintCallable, Category = "Card Management | Card Selection", meta = (WorldContext = "WorldContextObject"))
    static UK_CardsSelectionManagerComponent* GetCardsSelectionManager(UObject* WorldContextObject);

    // Helper function to select a card
    UFUNCTION(BlueprintCallable, Category = "Card Management | Card Selection", meta = (WorldContext = "WorldContextObject"))
    static void SelectCard(UObject* WorldContextObject, UCardObject* Card);

    // Helper function to deselect a card
    UFUNCTION(BlueprintCallable, Category = "Card Management | Card Selection", meta = (WorldContext = "WorldContextObject"))
    static void DeselectCard(UObject* WorldContextObject, UCardObject* Card);

    /** Player Attribute Management */

    // Function to get the player's attribute set
    UFUNCTION(BlueprintCallable, Category = "Card Management | Attributes", BlueprintPure)
    static const UK_CardsPlayerAttributeSet* GetPlayerAttributeSet(APlayerController* PlayerController);

    /** Player State Management */

    // PlayerStateComponent functions
    UFUNCTION(BlueprintCallable, Category = "Card Management | PlayerState", BlueprintPure)
    static UK_CardsPlayerStateComponent* GetCardsPlayerStateComponent(APlayerController* PlayerController);

    // Function to draw a card from one pile to another
    UFUNCTION(BlueprintCallable, Category = "Card Management")
    static UCardObject* DrawCardFromPile(APlayerController* PlayerController, FGameplayTag FromPileTag, FGameplayTag ToPileTag);

    UFUNCTION(BlueprintCallable, Category = "Card Management | PlayerState", BlueprintPure)
    static TArray<UCardObject*> GetPlayerDeck(APlayerController* PlayerController);

    UFUNCTION(BlueprintCallable, Category = "Card Management | PlayerState", BlueprintPure)
    static TArray<UCardObject*> GetPlayerDeckCardsByPileTag(APlayerController* PlayerController, FGameplayTag CurrentPileTag);

    UFUNCTION(BlueprintCallable, Category = "Card Management | PlayerState")
    static void TransferPlayerDeckCardToPile(APlayerController* PlayerController, UCardObject* Card, FGameplayTag NewPileTag);

    UFUNCTION(BlueprintCallable, Category = "Card Management | PlayerState")
    static int32 GetPlayerDeckCardCountByPileTag(APlayerController* PlayerController, FGameplayTag CurrentPileTag);

    /** Gameplay Tag Management */

    // Manage Tags On Player
    UFUNCTION(BlueprintCallable, Category = "Card Management | Ability System")
    static void AddGameplayTagToPlayer(APlayerController* PlayerController, const FGameplayTag& GameplayTag);

    UFUNCTION(BlueprintCallable, Category = "Card Management | GameplayTags")
    static void RemoveGameplayTagFromPlayer(APlayerController* PlayerController, FGameplayTag TagToRemove);

    /** Card Object Management */

    // Card Object functions
    UFUNCTION(BlueprintCallable, Category = "Card Management | Card Object")
    static TArray<UCardObject*> GetCardObjectsWithTagInContainer(APlayerController* PlayerController, FGameplayTag TagToSearch, ECardTagContainer TagContainer);

    // Function to create a card object
    UFUNCTION(BlueprintCallable, Category = "Card Management | Card Object")
    static UCardObject* CreateAndPopulateCardObject(TSubclassOf<UCardObject> CardClass, FCardData CardData, ECardState InitialState = ECardState::Passive, FGameplayTag InitialPileTag = FGameplayTag());

    /** 3D Card Management */

    // Function to create and register a card actor
    UFUNCTION(BlueprintCallable, Category = "Card Management | 3DCard")
    static A3DCard* CreateAndRegister3DCard(TSubclassOf<A3DCard> CardActorClass, UCardObject* CardObject, FVector SpawnLocation, FRotator SpawnRotation, AActor* Owner);

    // Function to remove and destroy a card actor
    UFUNCTION(BlueprintCallable, Category = "Card Management | 3DCard")
    static void RemoveAndDestroy3DCard(A3DCard* CardActor);

    /** Turn Manager Management */

    // Turn Manager functions
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    static UK_CardsTurnManagerComponent* GetCardsTurnManager();

    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    static bool IsCurrentPlayersTurn(AController* Controller);

    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    static ETurnType GetCurrentTurnType();

	// Pass TotalTurns to the caller
	UFUNCTION(BlueprintCallable, Category = "Turn Manager")
	static int32 GetTotalTurns();
	
    /** AI Controller Management */

    // AI Controller functions
    UFUNCTION(BlueprintCallable, Category = "Cards|AI")
    static AK_CardsAIController* GetCardsAIController();

    UFUNCTION(BlueprintCallable, Category = "Cards|AI")
	static void RemoveCardFromAIDeck(UCardObject* Card);

    /** Behavior Tree Management */

    // Behavior Tree functions
    UFUNCTION(BlueprintPure, Category = "AI|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
    static UCardObject* GetBlackboardValueAsCard(UBTNode* NodeOwner, const FBlackboardKeySelector& KeySelector);

    /** Card Manager Management */

    // Card Manager functions
    UFUNCTION(BlueprintCallable, Category = "Card Management | Card Manager")
    static UK_CardManager* GetCardManager();
};
