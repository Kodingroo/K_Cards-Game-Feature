// Copyright Isaac Hayward

#include "K_Task_AbortSequence.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "K_CardsAIController.h"

class AK_CardsAIController;

UK_Task_AbortSequence::UK_Task_AbortSequence()
{
	NodeName = "Should Abort Sequence";
}

EBTNodeResult::Type UK_Task_AbortSequence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp->GetValueAsBool(PlaySequenceKey.SelectedKeyName) == false || !AreThereCardsInAIDeck(OwnerComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaySequenceKey is false, aborting sequence."));
		BlackboardComp->SetValueAsInt(CurrentCardIndexKey.SelectedKeyName, 0); // Reset index if we have reached the end of the deck
		BlackboardComp->SetValueAsBool(PlaySequenceKey.SelectedKeyName, false); // Cause the Sequence to abort
		return EBTNodeResult::Aborted;
	}

	return EBTNodeResult::Succeeded;
}

bool UK_Task_AbortSequence::AreThereCardsInAIDeck(UBehaviorTreeComponent& OwnerComp)
{
	bool bAreThereCardsInAIDeck = false;
	// Ensure Creature Cards are on the AI Deck before looping through the AIDeck on the BT_AICardBehabior
	if (AK_CardsAIController* AIController = Cast<AK_CardsAIController>(OwnerComp.GetAIOwner()))
	{
		bAreThereCardsInAIDeck = AIController->AIDeck.Num() > 0;
	}
	
	return bAreThereCardsInAIDeck;
}

