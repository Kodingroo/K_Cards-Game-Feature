// #pragma once
//
// #include "CoreMinimal.h"
// #include "Abilities/Tasks/AbilityTask.h"
// #include "GameplayTagContainer.h"
// #include "K_AbilityTask_ReshuffleDeck.generated.h"
//
// UCLASS()
// class UK_AbilityTask_ReshuffleDeck : public UAbilityTask
// {
//     GENERATED_BODY()
//
// public:
//     DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReshuffleCompleted, bool, bHasCardsToDraw);
//
//     UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Check and Reshuffle Deck", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
//     static UK_AbilityTask_ReshuffleDeck* CheckAndReshuffleDeck(UGameplayAbility* OwningAbility, const TArray<UObject*>& Cards, FGameplayTag DiscardTag, FGameplayTag DrawTag);
//
//     virtual void Activate() override;
//
// protected:
//     void CheckPilesAndReshuffle();
//
//     TArray<UObject*> CardsToProcess;
//     FGameplayTag DiscardPileTag;
//     FGameplayTag DrawPileTag;
//
// public:
//     UPROPERTY(BlueprintAssignable)
//     FOnReshuffleCompleted OnReshuffleCompleted;
// };