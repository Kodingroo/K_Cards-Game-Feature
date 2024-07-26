// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTagContainer.h"
#include "K_ProcessCardsWithDelayAbilityTask.generated.h"

UCLASS()
class UK_ProcessCardsWithDelay : public UAbilityTask
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrayProcessed);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Process Cards With Delays", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UK_ProcessCardsWithDelay* ProcessCardsWithDelays(UGameplayAbility* OwningAbility, const TArray<UObject*>& CardsToProcess, FGameplayTagContainer EventTags, float DelayBetweenActivations, int32 MaxCardCount);

	virtual void Activate() override;

protected:
	void ProcessNextItem();
	void CheckAndReshuffle();
	void WaitForTagsRemoved();
	void OnTagsRemoved();
	void WaitForReshuffleComplete();

	UPROPERTY()
	TArray<UObject*> Objects;
	FGameplayTagContainer TriggerEventTags;
	FGameplayTag ReshuffleTag;
	float Delay;
	int32 MaxCount; // Maximum number of elements to process

	FTimerHandle TimerHandle;
	int32 CurrentIndex;

public:
	UPROPERTY(BlueprintAssignable)
	FOnArrayProcessed OnProcessed;

private:
	bool bIsReshuffleRequired;
};
