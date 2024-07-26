// Copyright Isaac Hayward

#include "K_PlayerHealthMagnitudeCalculation.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectTypes.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "GameplayTagContainer.h"

UK_PlayerHealthMagnitudeCalculation::UK_PlayerHealthMagnitudeCalculation()
    : HealthReductionAmount(1000.0f) // Default value, overridden in Blueprints
{
    HealthDefinition.AttributeToCapture = UK_CardsPlayerAttributeSet::GetHealthAttribute();
    HealthDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
    HealthDefinition.bSnapshot = false;

    RelevantAttributesToCapture.Add(HealthDefinition);
}

float UK_PlayerHealthMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    if (TargetTags && TargetTags->HasTagExact(FGameplayTag::RequestGameplayTag(FName("Player.Hungry"))))
    {
        // Reduce health by HealthReduction amount
        return -HealthReductionAmount;
    }
    
    return 0.0f;
}
