// Copyright Isaac Hayward

#include "K_CardsTurnManagerComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "K_CardManager.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/PlayerState.h"
#include "Player/LyraPlayerState.h"

UK_CardsTurnManagerComponent::UK_CardsTurnManagerComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer), AIBehaviorTree(nullptr)
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentTurnIndex = 0;
}

void UK_CardsTurnManagerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UK_CardsTurnManagerComponent::InitializeTurnManager()
{
    TurnOrder.Empty();
    CurrentTurnIndex = 0;

    // Add actors and AI to the turn order using GameState->PlayerArray
    int32 InitiativeIndex = 0;
    AGameStateBase* GameState = GetWorld()->GetGameState();
    if (GameState)
    {
        for (APlayerState* PlayerState : GameState->PlayerArray)
        {
            AddActorToTurnOrder(PlayerState->GetOwner(), InitiativeIndex);
            if (ALyraPlayerState* LyraPlayerState = Cast<ALyraPlayerState>(PlayerState))
            {
                if (UK_CardsPlayerStateComponent* PlayerStateComponent = LyraPlayerState->FindComponentByClass<UK_CardsPlayerStateComponent>())
                {
                    PlayerStateComponent->PlayerID = InitiativeIndex;
                    PlayerStateComponent->SetupStartingPlayerDeck();
                }
            }
            InitiativeIndex++;
        }
    }

    // Initialize all AI controllers specified in the Blueprint defaults
    for (TSubclassOf<AAIController> AIControllerClass : AIControllerClasses)
    {
        if (AIControllerClass && AIPawnClass)
        {
            APawn* AIPawn = GetWorld()->SpawnActor<APawn>(AIPawnClass);
            if (AIPawn)
            {
                AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass);
                if (AIController)
                {
                    AIController->Possess(AIPawn);
                    if (AIBehaviorTree)
                    {
                        AIController->RunBehaviorTree(AIBehaviorTree);
                    }
                    AddActorToTurnOrder(AIController, InitiativeIndex);
                    InitiativeIndex++;
                }
            }
        }
    }

    SortTurnOrder();
    
    // Set the Turn Manager in the Card Manager, currently for ResetManager purposes only
    UK_CardManager::GetInstance()->SetTurnManager(this);
}

void UK_CardsTurnManagerComponent::StartRound()
{
    StartNextTurn();
}

void UK_CardsTurnManagerComponent::StartNextTurn()
{
    if (TurnOrder.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("TurnOrder is empty"));
        return;
    }

    SortTurnOrder(); // Ensure the turn order is sorted before starting the next turn

    FTurnOrderEntry& CurrentEntry = TurnOrder[CurrentTurnIndex];
    if (AActor* CurrentActor = CurrentEntry.Actor)
    {
        // Check if the CurrentEntry.Actor is an AIController
        if (AAIController* AIController = Cast<AAIController>(CurrentActor))
        {
            // AIController is already running the BehaviorTree in InitializeTurnManager
            // Let the BehaviorTree manage the turn
        }
        else if (APlayerController* PlayerController = Cast<APlayerController>(CurrentActor))
        {
            if (ALyraPlayerState* PlayerState = PlayerController->GetPlayerState<ALyraPlayerState>())
            {
                if (ULyraAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetLyraAbilitySystemComponent())
                {
                    FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Turn.Start"));
                    AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));
                    TotalTurns++; 
                }
            }
        }
    }

    CurrentTurnIndex = (CurrentTurnIndex + 1) % TurnOrder.Num();
}

void UK_CardsTurnManagerComponent::EndTurn()
{
    AAIController* AIController = nullptr;
    APlayerController* PlayerController = nullptr;

    if (TurnOrder.IsValidIndex(CurrentTurnIndex))
    {
        AActor* CurrentActor = TurnOrder[CurrentTurnIndex].Actor;
        AIController = Cast<AAIController>(CurrentActor);
        PlayerController = Cast<APlayerController>(CurrentActor);
    }

    StartNextTurn();

    if (AIController)
    {
        AITurnCompleteDelegate.Broadcast(AIController);
    }
    else if (PlayerController)
    {
        PlayerTurnCompleteDelegate.Broadcast(PlayerController);
    }
}

void UK_CardsTurnManagerComponent::AddActorToTurnOrder(AActor* Actor, int32 Initiative)
{
    FTurnOrderEntry NewEntry;
    NewEntry.Actor = Actor;
    NewEntry.Initiative = Initiative;
    NewEntry.TurnType = Cast<AAIController>(Actor) ? ETurnType::AI : ETurnType::Player;

    TurnOrder.Add(NewEntry);
}

void UK_CardsTurnManagerComponent::RemoveActorFromTurnOrder(AActor* Actor)
{
    TurnOrder.RemoveAll([&](const FTurnOrderEntry& Entry)
    {
        return Entry.Actor == Actor;
    });
}

void UK_CardsTurnManagerComponent::SortTurnOrder()
{
    TurnOrder.Sort([](const FTurnOrderEntry& A, const FTurnOrderEntry& B)
    {
        return A.Initiative > B.Initiative;
    });
}

void UK_CardsTurnManagerComponent::HandleGameStart()
{
    InitializeTurnManager();
    StartRound();
}

void UK_CardsTurnManagerComponent::OnAIActionComplete(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
{
    EndTurn();
}

void UK_CardsTurnManagerComponent::OnPlayerActionComplete(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
    EndTurn();
}

void UK_CardsTurnManagerComponent::ResetTurnManager()
{
    TurnOrder.Empty();
    CurrentTurnIndex = 0;
}
