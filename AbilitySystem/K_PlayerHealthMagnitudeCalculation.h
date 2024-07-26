// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "K_PlayerHealthMagnitudeCalculation.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_PlayerHealthMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UK_PlayerHealthMagnitudeCalculation();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition HealthDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Reduction")
	float HealthReductionAmount;
};
