// Copyright Isaac Hayward

#include "K_HandLimitExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectTypes.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "Card/K_CardObject.h"

UK_HandLimitExecutionCalculation::UK_HandLimitExecutionCalculation()
    : BasketCardIncreaseAmount(2) // Default increase amount per basket card, can be overridden in the editor
{
    // Define the capture definitions (if needed for hand limit attributes)
    FGameplayEffectAttributeCaptureDefinition HandLimitDef(UK_CardsPlayerAttributeSet::GetHandLimitAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
    RelevantAttributesToCapture.Add(HandLimitDef);
}

void UK_HandLimitExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    AActor* TargetActor = ExecutionParams.GetOwningSpec().GetEffectContext().GetEffectCauser();
    if (!TargetActor)
    {
        return;
    }
    UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
    if (!AbilitySystemComponent)
    {
        return;
    }
    UK_CardsPlayerStateComponent* PlayerStateComponent = Cast<UK_CardsPlayerStateComponent>(AbilitySystemComponent->GetOwner()->GetComponentByClass(UK_CardsPlayerStateComponent::StaticClass()));
    if (!PlayerStateComponent)
    {
        return;
    }

    // Get the default starting hand limit from the player state component's data table
    float DefaultHandLimit = 0.0f;
    for (const FAttributeDefaults& StartingData : PlayerStateComponent->DefaultStartingAttributeData)
    {
        if (StartingData.Attributes && StartingData.DefaultStartingTable)
        {
            // Get the default starting hand limit from the Default Attribute Starting Table
            static const FName RowName(TEXT("K_CardsPlayerAttributeSet.HandLimit"));
            const FAttributeMetaData* AttributeData = StartingData.DefaultStartingTable->FindRow<FAttributeMetaData>(RowName, TEXT(""));

            if (AttributeData)
            {
                DefaultHandLimit = AttributeData->BaseValue;
            }
        }
    }

    // Check for Basket cards in the player's deck and on the table
    int32 BasketCardCount = 0;
    bool bCardRecentlyDestroyed = false;

    // Check for the temporary tags indicating the card was recently destroyed
    if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Ability.Card.Destroy"))))
    {
        bCardRecentlyDestroyed = true;
    }

    TArray<UCardObject*> PlayerCards = PlayerStateComponent->PlayerDeck;
    for (UCardObject* Card : PlayerCards)
    {
        // If the CardActor Exists, it is in play, either in motion to the table or if already on the table, check if it is selected and being destroyed
        if (Card->CardActor.Get() && (!bCardRecentlyDestroyed || Card->CardState != ECardState::Selected))
        {
            if (Card->Purpose.HasTag(FGameplayTag::RequestGameplayTag(FName("Card.Item.Basket"))))
            {
                BasketCardCount++;
            }
        }
    }

    // Calculate the total hand limit adjustment
    float TotalHandLimitAdjustment = DefaultHandLimit + (BasketCardCount * BasketCardIncreaseAmount);

    // Apply the calculated hand limit adjustment
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UK_CardsPlayerAttributeSet::GetHandLimitAttribute(), EGameplayModOp::Override, TotalHandLimitAdjustment));
}
