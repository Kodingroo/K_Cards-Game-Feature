// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "K_HungerHealthExecutionCalculation.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_HungerHealthExecutionCalculation : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UK_HungerHealthExecutionCalculation();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

    // These values can be set in the editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Health Calculation")
    float HungerReductionAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Health Calculation")
    float HealthReductionAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger Health Calculation")
    float HealthRestorationAmount;
};
