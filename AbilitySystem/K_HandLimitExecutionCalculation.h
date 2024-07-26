// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "K_HandLimitExecutionCalculation.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_HandLimitExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UK_HandLimitExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	// Increase amount per basket card, can be set in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand Limit Calculation")
	int32 BasketCardIncreaseAmount;
};
