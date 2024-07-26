// Copyright Isaac Hayward

#include "K_CardsDebugUtility.h"
#include "AbilitySystemComponent.h"
#include "K_CardsBlueprintFunctionLibrary.h"
#include "K_CardsTurnManagerComponent.h"
#include "Debugging/SlateDebugging.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "K_CardManager.h"
#include "K_CardsSelectionManagerComponent.h"
#include "Player/K_CardsPlayerStateComponent.h"

namespace MyCvars
{
    bool bDebugLogsEnabled = true;
    FAutoConsoleVariableRef CVarEnableDebugLogs(
        TEXT("Cards.DebugLogsEnabled"),
        bDebugLogsEnabled,
        TEXT("Enable or disable debug logs. true = Enabled, false = Disabled"),
        ECVF_Default
    );
}

TMap<FString, float> UK_CardsDebugUtility::LastLogTime;

UWorld* UK_CardsDebugUtility::GetWorldFromContext()
{
    UWorld* World = nullptr;
    if (GEngine && GEngine->GameViewport)
    {
        World = GEngine->GameViewport->GetWorld();
    }
    return World;
}

void UK_CardsDebugUtility::DebugPrint(const UObject* OptionalObject, const UObject* Location, const FString& Message, EDebugLogLevel DebugLogLevel, float Duration)
{
    if (!MyCvars::bDebugLogsEnabled)
    {
        UE_LOG(LogTemp, Log, TEXT("DebugPrint: Logging is disabled."));
        return;
    }

    if (!Location)
    {
        Location = OptionalObject;
    }

    FString Key = OptionalObject ? OptionalObject->GetName() + Message : Message;
    UWorld* World = GetWorldFromContext();
    if (!World) return;

    float CurrentTime = World->GetTimeSeconds();

    if (LastLogTime.Contains(Key) && (CurrentTime - LastLogTime[Key]) < 1.0f)
    {
        return;
    }

    LastLogTime.Add(Key, CurrentTime);

    FString LocationName = Location ? Location->GetName() : TEXT("Unknown");
    FString OptionalObjectName = OptionalObject ? OptionalObject->GetName() : TEXT("");

    FString FormattedMessage = FString::Printf(TEXT("%s - %s: %s"), *LocationName, *OptionalObjectName, *Message);

    FColor Color;
    switch (DebugLogLevel)
    {
        case EDebugLogLevel::Error:
            Color = FColor::Red;
            break;
        case EDebugLogLevel::Warning:
            Color = FColor::Yellow;
            break;
        case EDebugLogLevel::Critical:
            Color = FColor::Orange;
            break;
        case EDebugLogLevel::Verbose:
            Color = FColor::Cyan;
            break;
        case EDebugLogLevel::Info:
        default:
            Color = FColor::Green;
            break;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FormattedMessage);
    }

    switch (DebugLogLevel)
    {
        case EDebugLogLevel::Error:
            UE_LOG(LogTemp, Error, TEXT("%s"), *FormattedMessage);
            break;
        case EDebugLogLevel::Warning:
            UE_LOG(LogTemp, Warning, TEXT("%s"), *FormattedMessage);
            break;
        case EDebugLogLevel::Critical:
            UE_LOG(LogTemp, Error, TEXT("%s"), *FormattedMessage);
            break;
        case EDebugLogLevel::Verbose:
            UE_LOG(LogTemp, Verbose, TEXT("%s"), *FormattedMessage);
            break;
        case EDebugLogLevel::Info:
        default:
            UE_LOG(LogTemp, Log, TEXT("%s"), *FormattedMessage);
            break;
    }
}

void UK_CardsDebugUtility::LogRelevantTags(const UObject* Object)
{
    if (!Object || !MyCvars::bDebugLogsEnabled)
    {
        return;
    }

    FString Key = Object->GetName();
    FString TagDetails = FString::Printf(TEXT("Relevant Tags for %s:"), *Key);

    if (const AActor* Actor = Cast<AActor>(Object))
    {
        if (UAbilitySystemComponent* ASC = Actor->FindComponentByClass<UAbilitySystemComponent>())
        {
            FGameplayTagContainer OwnedTags;
            ASC->GetOwnedGameplayTags(OwnedTags);

            for (const FGameplayTag& Tag : OwnedTags)
            {
                TagDetails += FString::Printf(TEXT("\n- %s"), *Tag.ToString());
            }
        }
    }

    UWorld* World = GetWorldFromContext();
    if (!World) return;

    float CurrentTime = World->GetTimeSeconds();

    if (LastLogTime.Contains(Key) && (CurrentTime - LastLogTime[Key]) < 1.0f)
    {
        return;
    }

    LastLogTime.Add(Key, CurrentTime);

    FText ColoredMessage = FText::FromString(TagDetails);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, ColoredMessage.ToString());
    }

    UE_LOG(LogTemp, Log, TEXT("%s"), *TagDetails);
}

void UK_CardsDebugUtility::DebugTurnManagerExecutionFlow(const FString& Message, bool bDetailed, bool bConditional, const FString& Condition, bool bSnapshot)
{
    if (!MyCvars::bDebugLogsEnabled)
    {
        return;
    }

    if (bConditional && !Condition.IsEmpty())
    {
        // Add conditional logic here. For example, check if the condition matches the current game state or player's turn.
        // If the condition is not met, return without logging.
    }

    FString Key = TEXT("TurnManagerExecutionFlow");
    UWorld* World = GetWorldFromContext();
    if (!World) return;

    float CurrentTime = World->GetTimeSeconds();

    if (LastLogTime.Contains(Key) && (CurrentTime - LastLogTime[Key]) < 1.0f)
    {
        return;
    }

    LastLogTime.Add(Key, CurrentTime);

    FText ColoredMessage = FText::FromString(Message);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Blue, ColoredMessage.ToString());
    }

    UE_LOG(LogTemp, Log, TEXT("Turn Manager Execution Flow: %s"), *Message);

    if (bDetailed)
    {
        UE_LOG(LogTemp, Log, TEXT("Detailed Log: Entering %s"), *Message);
    }

    if (bSnapshot)
    {
        LogTurnManagerSnapshot();
    }
}

void UK_CardsDebugUtility::LogTurnManagerSnapshot()
{
    if (UK_CardsTurnManagerComponent* TurnManager = UK_CardsBlueprintFunctionLibrary::GetCardsTurnManager())
    {
        FString SnapshotDetails = TEXT("Turn Manager Snapshot:");

        // Log the current turn order
        for (const FTurnOrderEntry& Entry : TurnManager->TurnOrder)
        {
            SnapshotDetails += FString::Printf(TEXT("\n- Actor: %s, Initiative: %d"), *Entry.Actor->GetName(), Entry.Initiative);
        }

        FText ColoredMessage = FText::FromString(SnapshotDetails);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, ColoredMessage.ToString());
        }

        UE_LOG(LogTemp, Log, TEXT("%s"), *SnapshotDetails);
    }
}

void UK_CardsDebugUtility::PrintPlayerAttributes(AActor* PlayerActor)
{
    if (!PlayerActor) return;

    UAbilitySystemComponent* AbilitySystem = PlayerActor->FindComponentByClass<UAbilitySystemComponent>();
    if (AbilitySystem)
    {
        const UK_CardsPlayerAttributeSet* AttributeSet = AbilitySystem->GetSet<UK_CardsPlayerAttributeSet>();
        if (AttributeSet)
        {
            FString AttributeDetails = FString::Printf(TEXT("Player Attributes for %s:"), *PlayerActor->GetName());
            AttributeDetails += FString::Printf(TEXT("\n- Health: %f"), AttributeSet->GetHealth());
            AttributeDetails += FString::Printf(TEXT("\n- Hunger: %f"), AttributeSet->GetHunger());
            AttributeDetails += FString::Printf(TEXT("\n- Sanity: %f"), AttributeSet->GetSanity());
            AttributeDetails += FString::Printf(TEXT("\n- HandLimit: %f"), AttributeSet->GetHandLimit());
            AttributeDetails += FString::Printf(TEXT("\n- ExposureDamage: %f"), AttributeSet->GetExposureDamage());

            FText ColoredMessage = FText::FromString(AttributeDetails);
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, ColoredMessage.ToString());
            }

            UE_LOG(LogTemp, Log, TEXT("%s"), *AttributeDetails);
        }
    }
}

void UK_CardsDebugUtility::ListAllPilesContents(APlayerController* PlayerController, const UObject* Location)
{
    if (!PlayerController)
    {
        DebugPrint(Location, nullptr, TEXT("ListAllPilesContents: PlayerController not found"), EDebugLogLevel::Error);
        return;
    }

    UK_CardsPlayerStateComponent* PlayerStateComponent = UK_CardsBlueprintFunctionLibrary::GetCardsPlayerStateComponent(PlayerController);
    if (!PlayerStateComponent)
    {
        DebugPrint(Location, nullptr, TEXT("ListAllPilesContents: PlayerStateComponent not found"), EDebugLogLevel::Error);
        return;
    }

    TMap<FGameplayTag, TArray<UCardObject*>> CardPiles = PlayerStateComponent->GetAllCardPiles();
    for (const auto& Pile : CardPiles)
    {
        // Check if Location is valid and GetName is safe to call
        FString LocationName = Location ? Location->GetName() : TEXT("Unknown Location");
        FString PileContents = FString::Printf(TEXT("%s - Pile: %s\n"), *LocationName, *Pile.Key.ToString());
        
        for (const UCardObject* Card : Pile.Value)
        {
            if (Card)
            {
                FString CardName = Card->Name.IsEmpty() ? TEXT("Unnamed Card") : Card->Name.ToString();
                FString CurrentPileTag = Card->CurrentPileTag.IsValid() ? Card->CurrentPileTag.ToString() : TEXT("Invalid Tag");
                
                PileContents += FString::Printf(TEXT(" - %s (CurrentPileTag: %s)\n"), *CardName, *CurrentPileTag);
            }
        }

        // Log the pile contents with an optional location reference
        UE_LOG(LogTemp, Log, TEXT("%s"), *PileContents);
    }
}

void UK_CardsDebugUtility::LogCurrentSelections(const UObject* Location)
{
    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (!CardManager)
    {
        return;
    }
    
    UK_CardsSelectionManagerComponent* SelectionManager = CardManager->SelectionManagerComponent;
    if (!SelectionManager)
    {
        UE_LOG(LogTemp, Error, TEXT("%s - LogCurrentSelections: SelectionManager not found"), *Location->GetName());
        return;
    }

    const TArray<UCardObject*>& SelectedCards = SelectionManager->GetSelectedCards();
    if (SelectedCards.Num() == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("%s - LogCurrentSelections: No cards currently selected"), *Location->GetName());
        return;
    }

    for (const UCardObject* Card : SelectedCards)
    {
        if (Card)
        {
            UE_LOG(LogTemp, Log, TEXT("%s - Selected Card: %s, Pile: %s"), *Location->GetName(), *Card->GetName(), *Card->CurrentPileTag.ToString());
        }
    }
}