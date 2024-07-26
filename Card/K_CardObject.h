// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "K_CardData.h"
#include "GameplayTagContainer.h"
#include "K_CardObjectInterface.h"
#include "Engine/Texture2D.h"
#include "K_CardState.h"
#include "K_CardType.h"
#include "UObject/WeakObjectPtr.h"
#include "CommonUserWidget.h"
#include "K_CardObject.generated.h"

// Forward declaration to resolve circular dependency
class A3DCard;

/**
 * Represents a card object with properties, visual widgets, and gameplay functionality.
 */

UCLASS(Blueprintable)
class UCardObject : public UObject, public ICardObjectInterface
{
    GENERATED_BODY()

public:
    UCardObject();

    void InitializeVisualWidget();
    void RemoveVisualWidget();

    // Card properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
    ECardType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
    TSubclassOf<UCommonUserWidget> VisualWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
    TArray<FDataTableRowHandle> ToolTips;

    // Card tags
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer Purpose;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer PlaySettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer CardTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTagContainer InteractionConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
    FGameplayTag CurrentPileTag;

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

    // Usage-specific properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usage Properties")
    int32 MaxUsesPerTurn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Usage Properties")
    int32 UsesThisTurn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Usage Properties")
    int32 RemainingUses;

    // Card visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    UTexture2D* Portrait;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
    FSlateColor FrameTint;

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

    // Runtime properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
    ECardState CardState;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runtime")
    TWeakObjectPtr<A3DCard> CardActor; // Weak reference to the associated CardActor as it is stored in ActorToCardMap in the CardManager

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runtime")
    UCommonUserWidget* VisualWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Runtime")
    UCommonUserWidget* CardWidget;

    // Card functions
    UFUNCTION(BlueprintCallable, Category = "Functions")
    void PopulateCardData(const FCardData& CardData);

    UFUNCTION(BlueprintCallable, Category = "Functions")
    void UpdateVisualWidget();

    UFUNCTION(BlueprintCallable, Category = "Functions")
    void RegisterUse();

    UFUNCTION(BlueprintCallable, Category = "Functions")
    bool CanUse() const;

    UFUNCTION(BlueprintCallable, Category = "Functions")
    void ResetUses();

    void SetCardActor(A3DCard* InCardActor);
    void ClearCardActor();

protected:
    // Implement the interface function
    virtual FGameplayTag GetCurrentCardPile_Implementation() const override { return CurrentPileTag; }
    virtual void SetCurrentCardPile_Implementation(const FGameplayTag& PileName) override;
    virtual void ToggleCardGlow_Implementation(bool bEnable, FLinearColor GlowColor) override;

private:
    FCardData InternalCardData; // Struct to hold card data internally
};
