// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "K_Task_DealDamage.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_Task_DealDamage : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UK_Task_DealDamage();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector CurrentCardKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector PlayerActorKey;

	// Reference to the Gameplay Effect Class
	UPROPERTY(EditAnywhere, Category = "Gameplay Effect")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;
};
