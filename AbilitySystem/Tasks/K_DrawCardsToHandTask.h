#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTagContainer.h"
#include "K_DrawCardsToHandTask.generated.h"

UCLASS()
class UK_DrawCardsToHandTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDrawComplete);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Draw Cards To Hand", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UK_DrawCardsToHandTask* DrawCardsToHand(UGameplayAbility* OwningAbility, float DelayBetweenDraws, int32 HandLimit);

	virtual void Activate() override;

protected:
	void DrawNextCard();
	void CheckAndReshuffle();
	void WaitForReshuffleComplete();

	float Delay;
	int32 HandLimit;
	FGameplayTag ReshuffleTag;

	FTimerHandle TimerHandle;
	bool bIsReshuffleRequired;

public:
	UPROPERTY(BlueprintAssignable)
	FOnDrawComplete OnDrawComplete;
};
