// Copyright Isaac Hayward

#include "K_CardManager.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Card/K_3DCard.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "System/K_CardsTurnManagerComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UK_CardManager* UK_CardManager::Instance = nullptr;
FCriticalSection UK_CardManager::InstanceMutex;

UK_CardManager::UK_CardManager()
{
    SelectionManagerComponent = CreateDefaultSubobject<UK_CardsSelectionManagerComponent>(TEXT("SelectionManagerComponent"));
}

UK_CardManager* UK_CardManager::GetInstance()
{
    if (!Instance)
    {
        Initialize();
    }
    return Instance;
}

void UK_CardManager::Initialize()
{
    FScopeLock Lock(&InstanceMutex); // Lock the mutex to ensure thread safety
    if (!Instance)
    {
        Instance = NewObject<UK_CardManager>();
        Instance->AddToRoot(); // Prevent garbage collection
    }
}

void UK_CardManager::Shutdown()
{
    FScopeLock Lock(&InstanceMutex); // Lock the mutex to ensure thread safety
    if (Instance)
    {
        Instance->RemoveFromRoot();
        Instance = nullptr;
    }
}

void UK_CardManager::DealCardToPlayerState(UK_CardsPlayerStateComponent* PlayerState, const TSubclassOf<UCardObject> CardClass, FCardData CardData, ECardState InitialState, FGameplayTag InitialPileTag)
{
    if (PlayerState)
    {
        UCardObject* NewCardObject = CreateAndPopulateCardObject(CardClass, CardData, InitialState, InitialPileTag);
        if (NewCardObject)
        {
            PlayerState->AddCardToPile(NewCardObject, InitialPileTag);
        }
    }
}

A3DCard* UK_CardManager::CreateAndRegisterCardActor(TSubclassOf<A3DCard> CardActorClass, UCardObject* CardObject, FVector SpawnLocation, FRotator SpawnRotation, AActor* Owner)
{
    if (!CardObject || !Owner || !CardActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for CreateAndRegisterCardActor"));
        return nullptr;
    }

    UWorld* World = Owner->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid world context for CreateAndRegisterCardActor"));
        return nullptr;
    }

    // Pre-spawn setup
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawn the card actor
    A3DCard* NewCardActor = World->SpawnActorDeferred<A3DCard>(CardActorClass, FTransform(SpawnRotation, SpawnLocation), Owner);
    if (NewCardActor)
    {
        NewCardActor->SetCardObject(CardObject);
        RegisterCardActor(NewCardActor, CardObject);
        CardObject->SetCardActor(NewCardActor);
        NewCardActor->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
    return NewCardActor;
}

void UK_CardManager::ResetAllCardUses()
{
    for (const auto& Pair : ActorToCardMap)
    {
        if (UCardObject* CardObject = Pair.Value)
        {
            CardObject->ResetUses();
        }
    }
}

void UK_CardManager::RegisterCardActor(A3DCard* Actor, UCardObject* CardObject)
{
    if (Actor && CardObject)
    {
        ActorToCardMap.Add(Actor, CardObject);
    }
}

void UK_CardManager::UnregisterCardActor(A3DCard* Actor)
{
    ActorToCardMap.Remove(Actor);
}

UCardObject* UK_CardManager::GetCardObjectFromActor(A3DCard* Actor)
{
    UCardObject** FoundCardObject = ActorToCardMap.Find(Actor);
    return FoundCardObject ? *FoundCardObject : nullptr;
}

UCardObject* UK_CardManager::CreateAndPopulateCardObject(TSubclassOf<UCardObject> CardClass, FCardData CardData, ECardState InitialState, FGameplayTag InitialPileTag)
{
    UCardObject* NewCardObject = NewObject<UCardObject>(this, CardClass);
    if (NewCardObject)
    {
        NewCardObject->PopulateCardData(CardData);
        NewCardObject->CardState = InitialState;
        NewCardObject->Execute_SetCurrentCardPile(NewCardObject, InitialPileTag);
    }
    return NewCardObject;
}

void UK_CardManager::ResetManager()
{
    ActorToCardMap.Empty();

    if (SelectionManagerComponent)
    {
        SelectionManagerComponent->ClearSelection();
    }

    // Reset the Turn Manager if it's set
    if (TurnManager.IsValid())
    {
        TurnManager->ResetTurnManager();
        TurnManager = nullptr; // Clear the reference to prevent issues with GC
    }

    UWorld* World = GEngine->GameViewport->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is null in ResetAllWidgets"));
        return;
    }

    // Remove all children widgets from the viewport
    

    TArray<UUserWidget*> AllWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, AllWidgets, UUserWidget::StaticClass(), false);

    for (UUserWidget* Widget : AllWidgets)
    {
        if (Widget && Widget->IsInViewport())
        {
            // Remove all children widgets from the viewport
            UE_LOG(LogTemp, Log, TEXT("Removing widget: %s"), *Widget->GetName());
            Widget->RemoveFromViewport();
            Widget->Destruct();
        }
    }
    AllWidgets.Empty();
}

void UK_CardManager::SetTurnManager(UK_CardsTurnManagerComponent* InTurnManager)
{
    TurnManager = InTurnManager;
}
