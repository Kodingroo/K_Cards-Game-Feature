// Copyright Isaac Hayward

#include "K_ExposureLevelExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectTypes.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "Card/K_CardObject.h"

UK_ExposureLevelExecutionCalculation::UK_ExposureLevelExecutionCalculation()
    : ExposureReductionAmount(5.0f) // Default value, overridden in the editor
{
    // Define the capture definitions
    FGameplayEffectAttributeCaptureDefinition ExposureDef(UK_CardsPlayerAttributeSet::GetExposureDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
    RelevantAttributesToCapture.Add(ExposureDef);
}

void UK_ExposureLevelExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

    // Get the default starting exposure damage from the player state component's data table
    float DefaultExposureDamage = 0.0f;
    for (const FAttributeDefaults& StartingData : PlayerStateComponent->DefaultStartingAttributeData)
    {
        if (StartingData.Attributes && StartingData.DefaultStartingTable)
        {
            // Get the default starting exposure damage from the Default Attribute Starting Table
            static const FName RowName(TEXT("K_CardsPlayerAttributeSet.ExposureDamage"));
            const FAttributeMetaData* AttributeData = StartingData.DefaultStartingTable->FindRow<FAttributeMetaData>(RowName, TEXT(""));
            
            if (AttributeData)
            {
                DefaultExposureDamage = AttributeData->BaseValue;
            }
        }
    }

    // Check for Shelter and Fire cards in the player's deck and on the table
    bool bHasShelterCard = false;
    bool bHasFireCard = false;
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
            if (Card->Purpose.HasTag(FGameplayTag::RequestGameplayTag(FName("Card.Item.Shelter"))))
            {
                bHasShelterCard = true;
            }
            else if (Card->Purpose.HasTag(FGameplayTag::RequestGameplayTag(FName("Card.Item.Fire"))))
            {
                bHasFireCard = true;
            }
        }
    }

    // Calculate the total exposure adjustment
    float TotalExposureAdjustment = DefaultExposureDamage;
    if (bHasShelterCard)
    {
        TotalExposureAdjustment -= ExposureReductionAmount;
    }
    if (bHasFireCard)
    {
        TotalExposureAdjustment -= ExposureReductionAmount;
    }

    // Apply the calculated exposure adjustment
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UK_CardsPlayerAttributeSet::GetExposureDamageAttribute(), EGameplayModOp::Override, TotalExposureAdjustment));
}
