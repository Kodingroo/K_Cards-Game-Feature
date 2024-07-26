// Copyright Isaac Hayward

#include "K_DrawCardsToHandTask.h"
#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "System/K_CardsBlueprintFunctionLibrary.h"

UK_DrawCardsToHandTask* UK_DrawCardsToHandTask::DrawCardsToHand(UGameplayAbility* OwningAbility, float DelayBetweenDraws, int32 HandLimit)
{
    UK_DrawCardsToHandTask* Task = NewAbilityTask<UK_DrawCardsToHandTask>(OwningAbility);
    Task->Delay = DelayBetweenDraws;
    Task->HandLimit = HandLimit;
    Task->ReshuffleTag = FGameplayTag::RequestGameplayTag(FName("Ability.Deck.Reshuffle"));
    return Task;
}

void UK_DrawCardsToHandTask::Activate()
{
    Super::Activate();
    DrawNextCard();
}

void UK_DrawCardsToHandTask::DrawNextCard()
{
    APlayerController* PlayerController =  Ability->GetCurrentActorInfo()->PlayerController.Get();
    
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("DrawCardsToHandTask: PlayerController not found"));
        OnDrawComplete.Broadcast();
        EndTask();
        return;
    }
    
    UK_CardsPlayerStateComponent* PlayerStateComponent = UK_CardsBlueprintFunctionLibrary::GetCardsPlayerStateComponent(PlayerController);

    if (!PlayerStateComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("DrawCardsToHandTask: PlayerStateComponent not found"));
        OnDrawComplete.Broadcast();
        EndTask();
        return;
    }

    // Get the current hand count
    int32 CurrentHandCount = PlayerStateComponent->GetCardCountInPile(FGameplayTag::RequestGameplayTag(FName("Card.Pile.Hand")));

    // If hand limit is reached or there are no more cards to draw, finish the task
    if (CurrentHandCount >= HandLimit)
    {
        OnDrawComplete.Broadcast();
        EndTask();
        return;
    }

    // Draw a card from the top of the draw pile and add it to the hand
    UCardObject* DrawnCard = PlayerStateComponent->PopCardFromPile(FGameplayTag::RequestGameplayTag(FName("Card.Pile.Draw")));
    if (DrawnCard != nullptr)
    {
        // Perform the SetCurrentCardPile operation safely
        if (ICardObjectInterface* CardInterface = Cast<ICardObjectInterface>(DrawnCard))
        {
            CardInterface->Execute_SetCurrentCardPile(DrawnCard, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Hand")));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("CardObject does not implement ICardObjectInterface"));
        }
        
        PlayerStateComponent->AddCardToPile(DrawnCard, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Hand")));

        // Broadcast the event
        FGameplayEventData Payload;
        Payload.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Card.Add"));
        Payload.Target = GetOwnerActor();
        Payload.OptionalObject = DrawnCard;
        UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(GetOwnerActor(), true)->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Event.Card.Add")), &Payload);
        
        // Set the timer to draw the next card
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UK_DrawCardsToHandTask::DrawNextCard, Delay, false);
    }
    else
    {
        // If draw pile is empty and discard pile has cards, reshuffle
        int32 DiscardPileCount = PlayerStateComponent->GetCardCountInPile(FGameplayTag::RequestGameplayTag(FName("Card.Pile.Discard")));

        if (DiscardPileCount > 0)
        {
            CheckAndReshuffle();
        }
        else
        {
            // No cards left to draw
            OnDrawComplete.Broadcast();
            EndTask();
        }
    }
}

void UK_DrawCardsToHandTask::CheckAndReshuffle()
{
    bIsReshuffleRequired = true;
    // Activate the reshuffle ability
    UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(GetOwnerActor(), true)->TryActivateAbilitiesByTag(FGameplayTagContainer(ReshuffleTag));

    // Wait for the reshuffle to complete
    WaitForReshuffleComplete();
}

void UK_DrawCardsToHandTask::WaitForReshuffleComplete()
{
    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(GetOwnerActor(), true);
    if (ASC && ASC->HasMatchingGameplayTag(ReshuffleTag))
    {
        // Set a timer to check again after a short delay
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UK_DrawCardsToHandTask::WaitForReshuffleComplete, Delay, false);
    }
    else
    {
        bIsReshuffleRequired = false;
        DrawNextCard();
    }
}
