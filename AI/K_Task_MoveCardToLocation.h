// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "K_Task_MoveCardToLocation.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_Task_MoveCardToLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UK_Task_MoveCardToLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector CurrentCardKey;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector TargetLocation;

	FVector OriginalPosition;
	bool bIsMovingBack;

	void MoveCardToPosition(AActor* CardActor, const FVector& TargetPosition);
};
