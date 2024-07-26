#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "Card/K_CardObject.h"
#include "K_WaitForInterfaceHoverTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForInterfaceHoverDelegate, UCardObject*, HoverTarget);

class AActor;
struct FTimerHandle;

UCLASS()
class UAbilityTask_WaitForInterface_Hover : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FWaitForInterfaceHoverDelegate NewData;

	UPROPERTY(BlueprintAssignable)
	FWaitForInterfaceHoverDelegate NoData;

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitForInterface_Hover* WaitForInterface_Hover(UGameplayAbility* OwningAbility, float InteractionScanRate = 0.1, UClass* Interface = nullptr);

private:
	virtual void OnDestroy(bool AbilityEnded) override;

	void PerformHoverCheck();

	const UClass* Interface;
	FTransform StartTransform;
	FTransform EndTransform;
	float InteractionScanRate = 0.1;

	FTimerHandle TimerHandle;
};
