// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "K_Service_ProcessAIDeck.generated.h"

UCLASS()
class K_CARDSRUNTIME_API UK_Service_ProcessAIDeck : public UBTService
{
    GENERATED_BODY()

public:
    UK_Service_ProcessAIDeck();

protected:
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PlayerActorKey;
    
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector CurrentCardKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector CurrentCardIndexKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector PlaySequenceKey;
};
