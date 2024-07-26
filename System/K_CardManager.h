// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "K_CardsTurnManagerComponent.h"
#include "Card/K_CardObject.h"
#include "GameFramework/Actor.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "System/K_CardsSelectionManagerComponent.h"
#include "K_CardManager.generated.h"

/**
 * Manages card creation, registration, and interactions.
 * Provides singleton access and handles player state and turn manager integration.
 */

UCLASS(BlueprintType)
class UK_CardManager : public UObject
{
    GENERATED_BODY()

public:
    UK_CardManager();

    // Singleton access
    UFUNCTION(BlueprintCallable, Category="Card Management", meta=(DisplayName="Get Card Manager Instance"))
    static UK_CardManager* GetInstance();
    static void Initialize();
    static void Shutdown();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UK_CardsSelectionManagerComponent* SelectionManagerComponent;

    // Function to deal a card to a player's component
    UFUNCTION(BlueprintCallable, Category="Card Management | PlayerState")
    void DealCardToPlayerState(UK_CardsPlayerStateComponent* PlayerState, TSubclassOf<UCardObject> CardClass, FCardData CardData, ECardState InitialState, FGameplayTag InitialPileTag);

    A3DCard* CreateAndRegisterCardActor(TSubclassOf<A3DCard> CardActorClass, UCardObject* CardObject, FVector SpawnLocation, FRotator SpawnRotation, AActor* Owner);
    
    // Function to reset uses for all card objects
    UFUNCTION(BlueprintCallable, Category="Card Management")
    void ResetAllCardUses();
    
    // Function to register and unregister card actors
    UFUNCTION(BlueprintCallable, Category="Card Management | 3DCard")
    void RegisterCardActor(A3DCard* Actor, UCardObject* CardObject);

    UFUNCTION(BlueprintCallable, Category="Card Management | 3DCard")
    void UnregisterCardActor(A3DCard* Actor);

    UCardObject* GetCardObjectFromActor(A3DCard* Actor);

    // Function to reset the manager
    UFUNCTION(BlueprintCallable, Category="Card Management")
    void ResetManager();

    UCardObject* CreateAndPopulateCardObject(TSubclassOf<UCardObject> CardClass, FCardData CardData, ECardState InitialState, FGameplayTag InitialPileTag);

    // Set the reference to the Turn Manager
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void SetTurnManager(UK_CardsTurnManagerComponent* InTurnManager);

    UPROPERTY()
    TMap<A3DCard*, UCardObject*> ActorToCardMap;

private:
    static UK_CardManager* Instance;
    static FCriticalSection InstanceMutex; // Mutex for thread safety

    UPROPERTY()
    TWeakObjectPtr<UK_CardsTurnManagerComponent> TurnManager; // Weak reference to the Turn Manager
};
