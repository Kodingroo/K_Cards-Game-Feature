// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "K_PlayerHungerMagnitudeCalculation.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_PlayerHungerMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UK_PlayerHungerMagnitudeCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition HungerDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Reduction")
	float HungerReductionAmount;
};