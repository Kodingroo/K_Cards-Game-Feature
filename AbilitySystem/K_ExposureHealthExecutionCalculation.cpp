// Copyright Isaac Hayward

#include "K_ExposureHealthExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "Card/K_CardObject.h"
#include "System/K_CardsBlueprintFunctionLibrary.h"

UK_ExposureHealthExecutionCalculation::UK_ExposureHealthExecutionCalculation()
    : InitialExposureDamage(1000.0f) // Default value, overridden in the editor
    , DamageReductionPerReduceExposureCard(1000.0f) // Default value, overridden in the editor
{
    // Define the capture definitions
    FGameplayEffectAttributeCaptureDefinition HealthDef(UK_CardsPlayerAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
    RelevantAttributesToCapture.Add(HealthDef);
}

void UK_ExposureHealthExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    float Health = 0.0f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FGameplayEffectAttributeCaptureDefinition(UK_CardsPlayerAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Source, false), EvaluateParameters, Health);

    // Get the player state component from SourceASC
    APlayerController* PlayerController = Cast<APlayerController>(SourceASC->AbilityActorInfo.Get()->PlayerController);
    UK_CardsPlayerStateComponent* PlayerStateComponent = UK_CardsBlueprintFunctionLibrary::GetCardsPlayerStateComponent(PlayerController);
    if (!PlayerStateComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("UK_ExposureHealthExecutionCalculation::Execute_Implementation: PlayerStateComponent is null"));
        return;
    }

    // Collect unique shelter cards that are on the table
    TArray<FString> UniqueShelterCardNames;
    float TotalDamageReduction = 0.0f;

    for (UCardObject* Card : PlayerStateComponent->PlayerDeck)
    {
        if (Card->CardState == ECardState::OnTable &&
            Card->CardTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Card.ReduceExposure"))) &&
            !UniqueShelterCardNames.Contains(Card->Name.ToString()))
        {
            UniqueShelterCardNames.Add(Card->Name.ToString());
            TotalDamageReduction += DamageReductionPerReduceExposureCard;
        }
    }

    // Calculate the final health reduction based on initial exposure damage and damage reduction
    float FinalHealthReduction = InitialExposureDamage - TotalDamageReduction;
    if (FinalHealthReduction < 0.0f)
    {
        FinalHealthReduction = 0.0f;
    }
    
    // Apply the calculated damage to health
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UK_CardsPlayerAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -FinalHealthReduction));
}
