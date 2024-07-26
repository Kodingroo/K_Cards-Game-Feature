// Copyright Isaac Hayward

#include "K_Task_DealDamage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Card/K_CardObject.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/K_PlayerHealthMagnitudeCalculation.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Player/LyraPlayerState.h"

UK_Task_DealDamage::UK_Task_DealDamage()
{
    NodeName = "Deal Damage";
}

EBTNodeResult::Type UK_Task_DealDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController is nullptr"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("BlackboardComponent is nullptr"));
        return EBTNodeResult::Failed;
    }

    UObject* CurrentCardObject = BlackboardComp->GetValueAsObject(CurrentCardKey.SelectedKeyName);
    APlayerController* PlayerController = Cast<APlayerController>(AIController->GetWorld()->GetFirstPlayerController());

    if (!CurrentCardObject)
    {
        UE_LOG(LogTemp, Error, TEXT("CurrentCardObject is nullptr"));
        return EBTNodeResult::Failed;
    }

    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr"));
        return EBTNodeResult::Failed;
    }

    UCardObject* Card = Cast<UCardObject>(CurrentCardObject);
    if (!Card)
    {
        UE_LOG(LogTemp, Error, TEXT("Card is nullptr"));
        return EBTNodeResult::Failed;
    }

    ALyraPlayerState* PlayerState = PlayerController->GetPlayerState<ALyraPlayerState>();
    if (!PlayerState)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerState is nullptr"));
        return EBTNodeResult::Failed;
    }

    ULyraAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetLyraAbilitySystemComponent();
    if (!AbilitySystemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is nullptr"));
        return EBTNodeResult::Failed;
    }

    FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);

    if (!SpecHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("SpecHandle is not valid"));
        return EBTNodeResult::Failed;
    }

    FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
    if (!Spec)
    {
        UE_LOG(LogTemp, Error, TEXT("Spec is nullptr"));
        return EBTNodeResult::Failed;
    }

    Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Damage")), -Card->DamageValue);

    AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec, AbilitySystemComponent);

    UE_LOG(LogTemp, Warning, TEXT("Dealt %f damage to player with card %s"), Card->DamageValue, *Card->GetName());

    return EBTNodeResult::Succeeded;
}
