// Copyright Isaac Hayward

#include "K_HungerHealthExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Player/K_CardsPlayerAttributeSet.h"

UK_HungerHealthExecutionCalculation::UK_HungerHealthExecutionCalculation()
    : HungerReductionAmount(1000.0f) // Default value, overridden in the editor
    , HealthReductionAmount(1000.0f) // Default value, overridden in the editor
    , HealthRestorationAmount(1000.0f) // Default value, overridden in the editor
{
    // Define the capture definitions
    FGameplayEffectAttributeCaptureDefinition HungerDef(UK_CardsPlayerAttributeSet::GetHungerAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
    FGameplayEffectAttributeCaptureDefinition HealthDef(UK_CardsPlayerAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);

    RelevantAttributesToCapture.Add(HungerDef);
    RelevantAttributesToCapture.Add(HealthDef);
}

void UK_HungerHealthExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    float Hunger = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FGameplayEffectAttributeCaptureDefinition(UK_CardsPlayerAttributeSet::GetHungerAttribute(), EGameplayEffectAttributeCaptureSource::Source, false), EvaluateParameters, Hunger);

    // Apply health reduction if Hunger is zero or below
    if (Hunger <= 0.0f)
    {
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UK_CardsPlayerAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -HealthReductionAmount));
    }
    else
    {
        // Restore health if Hunger is above zero
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UK_CardsPlayerAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, HealthRestorationAmount));
    }

    // Reduce Hunger but ensure it doesn't go below zero
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UK_CardsPlayerAttributeSet::GetHungerAttribute(), EGameplayModOp::Additive, -FMath::Min(HungerReductionAmount, Hunger)));
}
