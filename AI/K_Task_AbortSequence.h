// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "K_Task_AbortSequence.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_Task_AbortSequence : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UK_Task_AbortSequence();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector PlaySequenceKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector CurrentCardIndexKey;

	bool AreThereCardsInAIDeck(UBehaviorTreeComponent& OwnerComp);
};
