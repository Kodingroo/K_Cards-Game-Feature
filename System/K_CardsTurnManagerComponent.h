// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "K_CardsTurnManagerComponent.generated.h"

class APlayerController;
class APawn;
class UBehaviorTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAITurnComplete, AAIController*, AIController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTurnComplete, APlayerController*, PlayerController);

// Add ENUM for player and AI turn types
UENUM(BlueprintType)
enum class ETurnType : uint8
{
    Player UMETA(DisplayName = "Player"),
    AI UMETA(DisplayName = "AI"),
    None UMETA(DisplayName = "None")
};

// Struct to hold actor and initiative values
USTRUCT(BlueprintType)
struct FTurnOrderEntry
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Order")
    AActor* Actor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Order")
    int32 Initiative;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Order")
    ETurnType TurnType;
};

/**
 * Manages the turn order and execution for players and AI.
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_CARDSRUNTIME_API UK_CardsTurnManagerComponent : public UGameStateComponent
{
    GENERATED_BODY()

public:
    UK_CardsTurnManagerComponent(const FObjectInitializer& ObjectInitializer);

    // Initialize the turn manager
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void InitializeTurnManager();

    // Start the round
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void StartRound();

    // Start the turn for the next actor
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void StartNextTurn();

    // End the current turn
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void EndTurn();

    // Add an actor to the turn order
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void AddActorToTurnOrder(AActor* Actor, int32 Initiative);

    // Remove an actor from the turn order
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void RemoveActorFromTurnOrder(AActor* Actor);

    UPROPERTY(BlueprintAssignable, Category = "Turn Manager")
    FOnAITurnComplete AITurnCompleteDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Turn Manager")
    FOnPlayerTurnComplete PlayerTurnCompleteDelegate;

    // Called to handle game start from Blueprints
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void HandleGameStart();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Manager")
    TArray<TSubclassOf<AAIController>> AIControllerClasses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Manager")
    TSubclassOf<APawn> AIPawnClass;

    // Behavior tree to assign to AI controllers
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Manager")
    UBehaviorTree* AIBehaviorTree;

    // Function to reset the turn manager
    UFUNCTION(BlueprintCallable, Category = "Turn Manager")
    void ResetTurnManager();

    // Sorted list of actors based on initiative
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Manager")
    TArray<FTurnOrderEntry> TurnOrder;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Manager")
    int32 CurrentTurnIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn Manager")
    int32 TotalTurns = 1;

protected:
    virtual void BeginPlay() override;

private:
    // Sort the turn order based on initiative values
    void SortTurnOrder();

    // Callbacks for AI and Player actions completion
    UFUNCTION()
    void OnAIActionComplete(FAIRequestID RequestID, const EPathFollowingResult::Type Result);

    UFUNCTION()
    void OnPlayerActionComplete(AActor* Actor, EEndPlayReason::Type EndPlayReason);
};
