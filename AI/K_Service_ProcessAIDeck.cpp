// Copyright Isaac Hayward

#include "K_Service_ProcessAIDeck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "K_CardsAIController.h"
#include "Engine/Engine.h"
#include "System/K_CardsBlueprintFunctionLibrary.h"

UK_Service_ProcessAIDeck::UK_Service_ProcessAIDeck()
{
    NodeName = "Process AI Deck";
    Interval = 0.0f; // Set to 0 to ensure it only runs once per OnBecomeRelevant
    RandomDeviation = 0.0f; // No deviation needed

    bNotifyBecomeRelevant = true;
}

void UK_Service_ProcessAIDeck::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);
    if (const UBlackboardData* BlackboardData = GetBlackboardAsset())
    {
        PlayerActorKey.ResolveSelectedKey(*BlackboardData);
        CurrentCardKey.ResolveSelectedKey(*BlackboardData);
        CurrentCardIndexKey.ResolveSelectedKey(*BlackboardData);
        PlaySequenceKey.ResolveSelectedKey(*BlackboardData);
    }
}

void UK_Service_ProcessAIDeck::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    AK_CardsAIController* AIController = Cast<AK_CardsAIController>(OwnerComp.GetAIOwner());
    if (BlackboardComp && AIController)
    {
        int32 CurrentCardIndex = BlackboardComp->GetValueAsInt(CurrentCardIndexKey.SelectedKeyName);

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            BlackboardComp->SetValueAsObject(PlayerActorKey.SelectedKeyName, PlayerController);
        }

        if (AIController->AIDeck.IsValidIndex(CurrentCardIndex))
        {
            UObject* CurrentCard = AIController->AIDeck[CurrentCardIndex];
            BlackboardComp->SetValueAsObject(CurrentCardKey.SelectedKeyName, CurrentCard);
            BlackboardComp->SetValueAsInt(CurrentCardIndexKey.SelectedKeyName, CurrentCardIndex + 1);
        }
        else
        {
            BlackboardComp->SetValueAsInt(CurrentCardIndexKey.SelectedKeyName, 0); // Reset index if we have reached the end of the deck
            BlackboardComp->SetValueAsBool(PlaySequenceKey.SelectedKeyName, false); // Cause the Sequence to abort
        }
    }
}
