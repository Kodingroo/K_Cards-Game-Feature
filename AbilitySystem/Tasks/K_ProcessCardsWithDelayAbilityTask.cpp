// Copyright Isaac Hayward

#include "K_ProcessCardsWithDelayAbilityTask.h"
#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "System/K_CardsBlueprintFunctionLibrary.h"

UK_ProcessCardsWithDelay* UK_ProcessCardsWithDelay::ProcessCardsWithDelays(UGameplayAbility* OwningAbility, const TArray<UObject*>& CardsToProcess, FGameplayTagContainer EventTags, float DelayBetweenActivations, int32 MaxCardCount)
{
    UK_ProcessCardsWithDelay* Task = NewAbilityTask<UK_ProcessCardsWithDelay>(OwningAbility);
    Task->Objects = CardsToProcess;
    Task->TriggerEventTags = EventTags;
    Task->Delay = DelayBetweenActivations;
    Task->MaxCount = MaxCardCount;
    Task->ReshuffleTag = FGameplayTag::RequestGameplayTag(FName("Ability.Deck.Reshuffle"));
    return Task;
}

void UK_ProcessCardsWithDelay::Activate()
{
    Super::Activate();
    CurrentIndex = 0;
    ProcessNextItem();
}

void UK_ProcessCardsWithDelay::ProcessNextItem()
{
    if (CurrentIndex < MaxCount && Objects.IsValidIndex(CurrentIndex))
    {
        UObject* ObjectToProcess = Objects[CurrentIndex++];
        
        // Triggering based on TriggerEventTags
        for (const FGameplayTag& EventTag : TriggerEventTags)
        {
            FGameplayEventData Payload;
            Payload.EventTag = EventTag;
            Payload.Target = GetOwnerActor();
            Payload.OptionalObject = ObjectToProcess;

            UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(GetOwnerActor(), true)->HandleGameplayEvent(EventTag, &Payload);
        }

        // Set the timer to process the next item
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UK_ProcessCardsWithDelay::ProcessNextItem, Delay, false);
    }
    else
    {
        CheckAndReshuffle();
    }
}

void UK_ProcessCardsWithDelay::CheckAndReshuffle()
{
    bIsReshuffleRequired = false;
    if (APlayerController* PlayerController = Cast<APlayerController>(GetOwnerActor()))
    {
        TArray<UCardObject*> DiscardPileCards = UK_CardsBlueprintFunctionLibrary::GetPlayerDeckCardsByPileTag(PlayerController, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Discard")));
        if (DiscardPileCards.Num() > 0)
        {
            bIsReshuffleRequired = true;
            // Activate the reshuffle ability
            UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(GetOwnerActor(), true)->TryActivateAbilitiesByTag(FGameplayTagContainer(ReshuffleTag));
        }
    }

    WaitForTagsRemoved();
}

void UK_ProcessCardsWithDelay::WaitForTagsRemoved()
{
    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(GetOwnerActor(), true);
    if (ASC)
    {
        bool bTagsStillActive = false;
        for (const FGameplayTag& EventTag : TriggerEventTags)
        {
            if (ASC->HasMatchingGameplayTag(EventTag))
            {
                bTagsStillActive = true;
                break;
            }
        }
        
        if (bTagsStillActive || bIsReshuffleRequired)
        {
            // Set a timer to check again after a short delay
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UK_ProcessCardsWithDelay::WaitForTagsRemoved, Delay, false);
        }
        else
        {
            OnTagsRemoved();
        }
    }
}

void UK_ProcessCardsWithDelay::OnTagsRemoved()
{
    if (bIsReshuffleRequired)
    {
        WaitForReshuffleComplete();
    }
    else
    {
        OnProcessed.Broadcast();
        EndTask();
    }
}

void UK_ProcessCardsWithDelay::WaitForReshuffleComplete()
{
    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(GetOwnerActor(), true);
    if (ASC && ASC->HasMatchingGameplayTag(ReshuffleTag))
    {
        // Set a timer to check again after a short delay
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UK_ProcessCardsWithDelay::WaitForReshuffleComplete, 0.1f, false);
    }
    else
    {
        OnProcessed.Broadcast();
        EndTask();
    }
}
