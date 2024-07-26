// // Copyright Isaac Hayward
//
// #include "K_AbilityTask_ReshuffleDeck.h"
// #include "GameFramework/Actor.h"
//
// UK_AbilityTask_ReshuffleDeck* UK_AbilityTask_ReshuffleDeck::CheckAndReshuffleDeck(UGameplayAbility* OwningAbility, const TArray<UObject*>& Cards, FGameplayTag DiscardTag, FGameplayTag DrawTag)
// {
// 	UK_AbilityTask_ReshuffleDeck* Task = NewAbilityTask<UK_AbilityTask_ReshuffleDeck>(OwningAbility);
// 	Task->CardsToProcess = Cards;
// 	Task->DiscardPileTag = DiscardTag;
// 	Task->DrawPileTag = DrawTag;
// 	return Task;
// }
//
// void UK_AbilityTask_ReshuffleDeck::Activate()
// {
// 	Super::Activate();
// 	CheckPilesAndReshuffle();
// }
//
// void UK_AbilityTask_ReshuffleDeck::CheckPilesAndReshuffle()
// {
// 	bool bHasCardsToDraw = false;
// 	bool bDiscardPileNotEmpty = false;
//
// 	// Check for cards in the draw pile
// 	for (UObject* Card : CardsToProcess)
// 	{
// 		ACard* CardActor = Cast<ACard>(Card);
// 		if (CardActor && CardActor->GetCurrentPile() == DrawPileTag)
// 		{
// 			bHasCardsToDraw = true;
// 			break;
// 		}
// 	}
//
// 	// If no cards in the draw pile, check the discard pile
// 	if (!bHasCardsToDraw)
// 	{
// 		for (UObject* Card : CardsToProcess)
// 		{
// 			ACard* CardActor = Cast<ACard>(Card);
// 			if (CardActor && CardActor->GetCurrentPile() == DiscardPileTag)
// 			{
// 				CardActor->SetCardPileTag(DrawPileTag);  // Move from discard to draw pile
// 				bDiscardPileNotEmpty = true;
// 			}
// 		}
// 	}
//
// 	bHasCardsToDraw = bHasCardsToDraw || bDiscardPileNotEmpty;
//
// 	if (ShouldBroadcastAbilityTaskDelegates())
// 	{
// 		OnReshuffleCompleted.Broadcast(bHasCardsToDraw);
// 	}
//
// 	EndTask();
// }