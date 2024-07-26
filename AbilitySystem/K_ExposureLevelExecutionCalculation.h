// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "K_ExposureLevelExecutionCalculation.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_ExposureLevelExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UK_ExposureLevelExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	// These values can be set in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exposure Level Calculation")
	float ExposureReductionAmount;
};
