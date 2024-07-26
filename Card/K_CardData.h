// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "K_CardType.h"
#include "K_CardVisualWidgetClass.h"
#include "Engine/Texture2D.h"
#include "K_CardData.generated.h"

/**
 * Struct that holds data for a card, used in data tables.
 */

USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    ECardType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    TArray<FDataTableRowHandle> ToolTips;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    TSubclassOf<UK_VisualWidgetClass> VisualWidgetClass;

    // Card tags
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer CardTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer Purpose;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer PlaySettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer InteractionConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTag PostUseEvent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTag EndTurnEvent;

    // Card values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
    float CoreValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
    float DamageValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
    float MitigationValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
    float RestoreValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
    float PowerValue;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
    float MaxUsesPerTurn;

    // Card visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    FSlateColor FrameTint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* Portrait;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* CardFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* CoreFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* DamageFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* MitigationFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* RestoreFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* PowerFrame;
};
