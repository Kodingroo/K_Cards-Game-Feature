// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "K_ExposureHealthExecutionCalculation.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_ExposureHealthExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UK_ExposureHealthExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	// These values can be set in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exposure Health Calculation")
	float InitialExposureDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exposure Health Calculation")
	float DamageReductionPerReduceExposureCard;
};
