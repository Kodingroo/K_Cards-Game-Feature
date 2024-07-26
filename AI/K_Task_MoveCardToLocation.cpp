// Copyright Isaac Hayward

#include "K_Task_MoveCardToLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

UK_Task_MoveCardToLocation::UK_Task_MoveCardToLocation()
{
    NodeName = "Move Card to Location";
    bNotifyTick = true;
    bIsMovingBack = false;
}

EBTNodeResult::Type UK_Task_MoveCardToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    UObject* CurrentCardObject = BlackboardComp->GetValueAsObject(CurrentCardKey.SelectedKeyName);
    AActor* CardActor = Cast<AActor>(CurrentCardObject);

    if (!CardActor)
    {
        return EBTNodeResult::Failed;
    }

    OriginalPosition = CardActor->GetActorLocation();
    MoveCardToPosition(CardActor, TargetLocation);

    return EBTNodeResult::InProgress;
}

void UK_Task_MoveCardToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UObject* CurrentCardObject = BlackboardComp->GetValueAsObject(CurrentCardKey.SelectedKeyName);
    AActor* CardActor = Cast<AActor>(CurrentCardObject);

    if (CardActor)
    {
        FVector CurrentPosition = CardActor->GetActorLocation();

        if (bIsMovingBack)
        {
            MoveCardToPosition(CardActor, OriginalPosition);

            if (FVector::Dist(CurrentPosition, OriginalPosition) < 1.0f)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }
        }
        else
        {
            MoveCardToPosition(CardActor, TargetLocation);

            if (FVector::Dist(CurrentPosition, TargetLocation) < 1.0f)
            {
                bIsMovingBack = true;
            }
        }
    }
    else
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
}

void UK_Task_MoveCardToLocation::MoveCardToPosition(AActor* CardActor, const FVector& TargetPosition)
{
    FVector CurrentPosition = CardActor->GetActorLocation();
    FVector NewPosition = FMath::VInterpTo(CurrentPosition, TargetPosition, GetWorld()->GetDeltaSeconds(), 5.0f);
    CardActor->SetActorLocation(NewPosition);
}
