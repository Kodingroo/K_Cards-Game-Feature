// Copyright Isaac Hayward

#include "K_CardsBlueprintFunctionLibrary.h"
#include "K_CardManager.h"
#include "K_CardsTurnManagerComponent.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AI/K_CardsAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTNode.h"
#include "Card/K_3DCard.h"
#include "Card/K_CardSearchTagContainer.h"
#include "Engine/World.h"
#include "Misc/RuntimeErrors.h"
#include "Player/LyraPlayerState.h"

UK_CardsSelectionManagerComponent* UK_CardsBlueprintFunctionLibrary::GetCardsSelectionManager(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (CardManager)
    {
        return CardManager->SelectionManagerComponent;
    }

    return nullptr;
}

void UK_CardsBlueprintFunctionLibrary::SelectCard(UObject* WorldContextObject, UCardObject* Card)
{
    if (!WorldContextObject || !Card)
    {
        UE_LOG(LogTemp, Warning, TEXT("WorldContextObject or Card is null in SelectCard"));
        return;
    }

    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (CardManager && CardManager->SelectionManagerComponent)
    {
        CardManager->SelectionManagerComponent->SelectCard(Card);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CardManager or SelectionManagerComponent is null in SelectCard"));
    }
}

void UK_CardsBlueprintFunctionLibrary::DeselectCard(UObject* WorldContextObject, UCardObject* Card)
{
    if (!WorldContextObject || !Card)
    {
        UE_LOG(LogTemp, Warning, TEXT("WorldContextObject or Card is null in DeselectCard"));
        return;
    }

    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (CardManager && CardManager->SelectionManagerComponent)
    {
        CardManager->SelectionManagerComponent->DeselectCard(Card);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CardManager or SelectionManagerComponent is null in DeselectCard"));
    }
}

const UK_CardsPlayerAttributeSet* UK_CardsBlueprintFunctionLibrary::GetPlayerAttributeSet(APlayerController* PlayerController)
{
    if (UK_CardsPlayerStateComponent* PlayerStateComponent = GetCardsPlayerStateComponent(PlayerController))
    {
        if (UAbilitySystemComponent* ASC = PlayerStateComponent->GetOwner()->FindComponentByClass<UAbilitySystemComponent>())
        {
            return ASC->GetSet<UK_CardsPlayerAttributeSet>();
        }
    }
    return nullptr;
}

UK_CardsPlayerStateComponent* UK_CardsBlueprintFunctionLibrary::GetCardsPlayerStateComponent(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        ALyraPlayerState* LyraPlayerState = PlayerController->GetPlayerState<ALyraPlayerState>();
        if (LyraPlayerState)
        {
            return LyraPlayerState->FindComponentByClass<UK_CardsPlayerStateComponent>();
        }
    }
    return nullptr;
}

TArray<UCardObject*> UK_CardsBlueprintFunctionLibrary::GetPlayerDeck(APlayerController* PlayerController)
{
    if (UK_CardsPlayerStateComponent* PlayerStateComponent = GetCardsPlayerStateComponent(PlayerController))
    {
        return PlayerStateComponent->PlayerDeck;
    }
    return TArray<UCardObject*>();
}

TArray<UCardObject*> UK_CardsBlueprintFunctionLibrary::GetPlayerDeckCardsByPileTag(APlayerController* PlayerController, FGameplayTag CurrentPileTag)
{
    if (UK_CardsPlayerStateComponent* PlayerStateComponent = GetCardsPlayerStateComponent(PlayerController))
    {
        return PlayerStateComponent->GetCardsInPile(CurrentPileTag);
    }
    return TArray<UCardObject*>();
}

void UK_CardsBlueprintFunctionLibrary::TransferPlayerDeckCardToPile(APlayerController* PlayerController, UCardObject* Card, FGameplayTag NewPileTag)
{
    if (UK_CardsPlayerStateComponent* PlayerStateComponent = GetCardsPlayerStateComponent(PlayerController))
    {
        PlayerStateComponent->TransferCardToPile(Card, NewPileTag);
    }
}

int32 UK_CardsBlueprintFunctionLibrary::GetPlayerDeckCardCountByPileTag(APlayerController* PlayerController, FGameplayTag CurrentPileTag)
{
    if (const UK_CardsPlayerStateComponent* PlayerStateComponent = GetCardsPlayerStateComponent(PlayerController))
    {
        return PlayerStateComponent->GetCardCountInPile(CurrentPileTag);
    }
    return 0;
}

TArray<UCardObject*> UK_CardsBlueprintFunctionLibrary::GetCardObjectsWithTagInContainer(APlayerController* PlayerController, FGameplayTag TagToSearch, ECardTagContainer TagContainer)
{
    TArray<UCardObject*> MatchingCards;

    if (const UK_CardsPlayerStateComponent* PlayerStateComponent = GetCardsPlayerStateComponent(PlayerController))
    {
        const TArray<UCardObject*>& PlayerDeck = PlayerStateComponent->PlayerDeck;
        for (UCardObject* Card : PlayerDeck)
        {
            if (Card)
            {
                bool bTagFound = false;

                switch (TagContainer)
                {
                case ECardTagContainer::Purpose:
                    bTagFound = Card->Purpose.HasTag(TagToSearch);
                    break;
                case ECardTagContainer::PlaySettings:
                    bTagFound = Card->PlaySettings.HasTag(TagToSearch);
                    break;
                case ECardTagContainer::CardTags:
                    bTagFound = Card->CardTags.HasTag(TagToSearch);
                    break;
                case ECardTagContainer::InteractionConditions:
                    bTagFound = Card->InteractionConditions.HasTag(TagToSearch);
                    break;
                default:
                    break;
                }

                if (bTagFound)
                {
                    MatchingCards.Add(Card);
                }
            }
        }
    }

    return MatchingCards;
}

void UK_CardsBlueprintFunctionLibrary::AddGameplayTagToPlayer(APlayerController* PlayerController, const FGameplayTag& GameplayTag)
{
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("AddGameplayTagToPlayer: PlayerController is null"));
        return;
    }

    ALyraPlayerState* LyraPlayerState = PlayerController->GetPlayerState<ALyraPlayerState>();
    if (!LyraPlayerState)
    {
        UE_LOG(LogTemp, Error, TEXT("AddGameplayTagToPlayer: LyraPlayerState is null"));
        return;
    }

    ULyraAbilitySystemComponent* AbilitySystemComponent = Cast<ULyraAbilitySystemComponent>(LyraPlayerState->GetAbilitySystemComponent());
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("AddGameplayTagToPlayer: AbilitySystemComponent is null"));
        return;
    }

    AbilitySystemComponent->AddLooseGameplayTag(GameplayTag);
}

void UK_CardsBlueprintFunctionLibrary::RemoveGameplayTagFromPlayer(APlayerController* PlayerController, FGameplayTag TagToRemove)
{
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("RemoveGameplayTagFromPlayer: PlayerController is null"));
        return;
    }

    ALyraPlayerState* LyraPlayerState = PlayerController->GetPlayerState<ALyraPlayerState>();
    if (!LyraPlayerState)
    {
        UE_LOG(LogTemp, Error, TEXT("RemoveGameplayTagFromPlayer: LyraPlayerState is null"));
        return;
    }

    ULyraAbilitySystemComponent* AbilitySystemComponent = Cast<ULyraAbilitySystemComponent>(LyraPlayerState->GetAbilitySystemComponent());
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("RemoveGameplayTagFromPlayer: AbilitySystemComponent is null"));
        return;
    }

    // Remove the tag
    if (AbilitySystemComponent->HasMatchingGameplayTag(TagToRemove))
    {
        AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
    }
}

UK_CardsTurnManagerComponent* UK_CardsBlueprintFunctionLibrary::GetCardsTurnManager()
{
    UWorld* World = nullptr;

    if (GEngine && GEngine->GameViewport)
    {
        World = GEngine->GameViewport->GetWorld();
    }

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to find a valid WorldContextObject"));
        return nullptr;
    }

    AGameStateBase* GameState = World->GetGameState();
    if (!GameState)
    {
        return nullptr;
    }

    return GameState->FindComponentByClass<UK_CardsTurnManagerComponent>();
}

bool UK_CardsBlueprintFunctionLibrary::IsCurrentPlayersTurn(AController* Controller)
{
    if (UK_CardsTurnManagerComponent* TurnManager = GetCardsTurnManager())
    {
        if (TurnManager->CurrentTurnIndex >= 0 && TurnManager->CurrentTurnIndex < TurnManager->TurnOrder.Num())
        {
            FTurnOrderEntry CurrentTurnEntry = TurnManager->TurnOrder[TurnManager->CurrentTurnIndex];
            if (CurrentTurnEntry.Actor)
            {
                return CurrentTurnEntry.Actor == Controller;
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("CurrentTurnIndex is out of bounds."));
        }
    }
    return false;
}

ETurnType UK_CardsBlueprintFunctionLibrary::GetCurrentTurnType()
{
    UK_CardsTurnManagerComponent* TurnManager = GetCardsTurnManager();
    if (TurnManager && TurnManager->TurnOrder.IsValidIndex(TurnManager->CurrentTurnIndex))
    {
        FTurnOrderEntry CurrentTurnEntry = TurnManager->TurnOrder[TurnManager->CurrentTurnIndex];
        return CurrentTurnEntry.TurnType;
    }

    return ETurnType::None;
}

int32 UK_CardsBlueprintFunctionLibrary::GetTotalTurns()
{
    UK_CardsTurnManagerComponent* TurnManager = GetCardsTurnManager();
    if (TurnManager)
    {
        return TurnManager->TotalTurns;
    }
    return 0;
}

AK_CardsAIController* UK_CardsBlueprintFunctionLibrary::GetCardsAIController()
{
    // Get the AI Controller from the Turn Manager
    if (UK_CardsTurnManagerComponent* TurnManager = GetCardsTurnManager())
    {
        if (TurnManager->TurnOrder.IsValidIndex(TurnManager->CurrentTurnIndex))
        {
            FTurnOrderEntry CurrentTurnEntry = TurnManager->TurnOrder[TurnManager->CurrentTurnIndex];
            if (CurrentTurnEntry.Actor)
            {
                return Cast<AK_CardsAIController>(CurrentTurnEntry.Actor);
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Failed to find AI Controller in UK_CardsBlueprintFunctionLibrary because CurrentTurnEntry.Actor is null"));
    return nullptr;
}

void UK_CardsBlueprintFunctionLibrary::RemoveCardFromAIDeck(UCardObject* Card)
{
    UK_CardsTurnManagerComponent* TurnManager = GetCardsTurnManager();
    if (!TurnManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find TurnManager in UK_CardsBlueprintFunctionLibrary"));
        return;
    }
    
    // Iterate through the TurnOrder to find the AI controller's deck
    for (const FTurnOrderEntry& TurnEntry : TurnManager->TurnOrder)
    {
        if (TurnEntry.TurnType == ETurnType::AI && Card)
        {
            if (AK_CardsAIController* AIController = Cast<AK_CardsAIController>(TurnEntry.Actor))
            {
                AIController->RemoveCardFromAIDeck(Card);
                return;
            }
        }
    }
}

UCardObject* UK_CardsBlueprintFunctionLibrary::GetBlackboardValueAsCard(UBTNode* NodeOwner, const FBlackboardKeySelector& KeySelector)
{
    ensureAsRuntimeWarning((NodeOwner != nullptr) && (NodeOwner->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint)));

    UBehaviorTreeComponent* OwnerComp = NodeOwner ? Cast<UBehaviorTreeComponent>(NodeOwner->GetOuter()) : nullptr;
    ensureAsRuntimeWarning(OwnerComp != nullptr);

    UBlackboardComponent* BlackboardComp = OwnerComp->GetBlackboardComponent();
    return BlackboardComp ? Cast<UCardObject>(BlackboardComp->GetValueAsObject(KeySelector.SelectedKeyName)) : nullptr;
}

UK_CardManager* UK_CardsBlueprintFunctionLibrary::GetCardManager()
{
    return UK_CardManager::GetInstance();
}

UCardObject* UK_CardsBlueprintFunctionLibrary::CreateAndPopulateCardObject(TSubclassOf<UCardObject> CardClass, FCardData CardData, ECardState InitialState, FGameplayTag InitialPileTag)
{
    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (CardManager)
    {
        return CardManager->CreateAndPopulateCardObject(CardClass, CardData, InitialState, InitialPileTag);
    }
    return nullptr;
}

A3DCard* UK_CardsBlueprintFunctionLibrary::CreateAndRegister3DCard(TSubclassOf<A3DCard> CardActorClass, UCardObject* CardObject, FVector SpawnLocation, FRotator SpawnRotation, AActor* Owner)
{
    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (CardManager)
    {
        return CardManager->CreateAndRegisterCardActor(CardActorClass, CardObject, SpawnLocation, SpawnRotation, Owner);
    }
    return nullptr;
}

void UK_CardsBlueprintFunctionLibrary::RemoveAndDestroy3DCard(A3DCard* CardActor)
{
    if (!CardActor)
    {
        return;
    }

    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (CardManager)
    {
        UCardObject* CardObject = CardManager->GetCardObjectFromActor(CardActor);
        if (CardObject)
        {
            // Set the CardState to Passive after the 3D card has completed any movement and is ready to be destroyed
            CardObject->CardState = ECardState::Passive;
        }
        
        CardManager->UnregisterCardActor(CardActor);

        if (CardActor->IsValidLowLevel())
        {
            CardActor->Destroy();
        }
    }
}

UCardObject* UK_CardsBlueprintFunctionLibrary::DrawCardFromPile(APlayerController* PlayerController, FGameplayTag FromPileTag, FGameplayTag ToPileTag)
{
    if (!PlayerController)
    {
        return nullptr;
    }

    UK_CardsPlayerStateComponent* PlayerState = GetCardsPlayerStateComponent(PlayerController);
    if (!PlayerState)
    {
        return nullptr;
    }

    // Remove the card from the draw pile
    UCardObject* CardToDraw = PlayerState->PopCardFromPile(FromPileTag);
    if (!CardToDraw)
    {
        return nullptr;
    }

    // Add the card to the hand pile
    PlayerState->AddCardToPile(CardToDraw, ToPileTag);
    CardToDraw->Execute_SetCurrentCardPile(CardToDraw, ToPileTag);

    return CardToDraw;
}
