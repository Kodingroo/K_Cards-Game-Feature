// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "K_Task_SetBlackboardBool.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_Task_SetBlackboardBool : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UK_Task_SetBlackboardBool();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardKey;

	UPROPERTY(EditAnywhere, Category = "Task")
	bool bValueToSet;
};
