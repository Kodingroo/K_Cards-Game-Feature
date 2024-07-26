// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "K_CardsDebugUtility.generated.h"

namespace MyCvars
{
    extern bool bDebugLogsEnabled;
    extern FAutoConsoleVariableRef CVarEnableDebugLogs;
}

UENUM(BlueprintType)
enum class EDebugLogLevel : uint8
{
    Info        UMETA(DisplayName = "Info"),
    Warning     UMETA(DisplayName = "Warning"),
    Error       UMETA(DisplayName = "Error"),
    Critical    UMETA(DisplayName = "Critical"),
    Verbose     UMETA(DisplayName = "Verbose")
};

/**
 * Provides debug utilities for the Cards Game Feature.
 */
UCLASS()
class K_CARDSRUNTIME_API UK_CardsDebugUtility : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Logs a debug message with optional object and location
    UFUNCTION(BlueprintCallable, Category = "Cards Debug")
    static void DebugPrint(const UObject* OptionalObject, const UObject* Location, const FString& Message, EDebugLogLevel DebugLogLevel = EDebugLogLevel::Info, float Duration = 7.0f);

    UFUNCTION(BlueprintCallable, Category = "Cards Debug")
    static void LogRelevantTags(const UObject* Object);

    /** Turn Manager Debugging */
    // Logs turn manager execution flow with optional details and conditions
    UFUNCTION(BlueprintCallable, Category = "Cards Debug")
    static void DebugTurnManagerExecutionFlow(const FString& Message, bool bDetailed = false, bool bConditional = false, const FString& Condition = TEXT(""), bool bSnapshot = false);

    /** Attribute Set Debugging */
    UFUNCTION(BlueprintCallable, Category = "Cards Debug")
    static void PrintPlayerAttributes(AActor* PlayerActor);

    // Lists contents of all piles with a custom message from BP
    UFUNCTION(BlueprintCallable, Category = "Cards Debug")
    static void ListAllPilesContents(APlayerController* PlayerController, const UObject* Location = nullptr);

    // Logs current selections in the Selection Manager
    UFUNCTION(BlueprintCallable, Category = "Cards Debug")
    static void LogCurrentSelections(const UObject* Location);

private:
    static bool bDebugLogsEnabled;
    static void LogTurnManagerSnapshot();
    static TMap<FString, float> LastLogTime;
    static UWorld* GetWorldFromContext();
};
