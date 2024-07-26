// Copyright Isaac Hayward

#include "K_PlayerHungerMagnitudeCalculation.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "GameplayTagContainer.h"

UK_PlayerHungerMagnitudeCalculation::UK_PlayerHungerMagnitudeCalculation()
    : HungerReductionAmount(1000.0f) // Default value, overridden in Blueprints
{
    HungerDefinition.AttributeToCapture = UK_CardsPlayerAttributeSet::GetHungerAttribute();
    HungerDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
    HungerDefinition.bSnapshot = false;

    RelevantAttributesToCapture.Add(HungerDefinition);
}

float UK_PlayerHungerMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    FAggregatorEvaluateParameters EvaluationParameters;
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float Hunger = 0.0f;

    // Store the captured attribute magnitude in Hunger
    if (GetCapturedAttributeMagnitude(HungerDefinition, Spec, EvaluationParameters, Hunger))
    {
        UE_LOG(LogTemp, Log, TEXT("Hunger captured: %f"), Hunger);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to capture Hunger attribute"));
        return 0.0f; // Early exit if capture fails
    }

    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(Spec.GetEffectContext().GetInstigator());
    if (!ASC)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get ASC from Spec.GetEffectContext().GetInstigator() in UK_PlayerHungerMagnitudeCalculation::CalculateBaseMagnitude_Implementation"));
        return 0.0f;
    }
    
    // Apply "Hungry" tag if hunger reaches zero
    if (Hunger <= 0.0f)
    {
        ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Hungry")));
    }
    else
    {
        // Remove "Hungry" tag if hunger is above zero
        ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Hungry")));
    }
    
    // Reduce Hunger but ensure it doesn't go below zero
    return -FMath::Min(HungerReductionAmount, Hunger);
}