#include "K_WaitForInterfaceHoverTask.h"
#include "Engine/TimerHandle.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Card/K_3DCard.h"
#include "Card/K_CardObject.h"
#include "System/K_CardManager.h"

#define CardInteraction_Channel ECC_GameTraceChannel7  /*Card_Interaction*/

UAbilityTask_WaitForInterface_Hover::UAbilityTask_WaitForInterface_Hover(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer), Interface(nullptr)
{
}

UAbilityTask_WaitForInterface_Hover* UAbilityTask_WaitForInterface_Hover::WaitForInterface_Hover(
    UGameplayAbility* OwningAbility,
    float InteractionScanRate,
    UClass* Interface)
{
    UAbilityTask_WaitForInterface_Hover* MyObj = NewAbilityTask<UAbilityTask_WaitForInterface_Hover>(OwningAbility);
    MyObj->InteractionScanRate = InteractionScanRate;
    MyObj->Interface = Interface;
    
    return MyObj;
}

void UAbilityTask_WaitForInterface_Hover::Activate()
{
    SetWaitingOnAvatar();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformHoverCheck, InteractionScanRate, true);
}

void UAbilityTask_WaitForInterface_Hover::OnDestroy(const bool AbilityEnded)
{
    Super::OnDestroy(AbilityEnded);

    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UAbilityTask_WaitForInterface_Hover::PerformHoverCheck()
{
    AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
    if (!AvatarActor)
    {
        NoData.Broadcast(nullptr);
        return;
    }

    bool bTraceComplex = true;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(AvatarActor);

    FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_WaitForInterface_Hover), bTraceComplex);
    Params.AddIgnoredActors(ActorsToIgnore);

    FVector MouseLocation, MouseDirection;
    Ability->GetCurrentActorInfo()->PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

    StartTransform.SetLocation(MouseLocation);
    StartTransform.SetRotation(MouseDirection.Rotation().Quaternion());
    EndTransform = StartTransform;
    EndTransform.AddToTranslation(MouseDirection * 100000);

    FVector TraceStart = StartTransform.GetLocation();
    FVector TraceEnd = EndTransform.GetLocation();

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CardInteraction_Channel, Params);

    const AActor* HitActor = HitResult.GetActor();

    UK_CardManager* CardManager = UK_CardManager::GetInstance();
    if (!CardManager)
    {
        NoData.Broadcast(nullptr);
        return;
    }
    
    UK_CardsSelectionManagerComponent* SelectionManager = CardManager->SelectionManagerComponent;
    if (!SelectionManager)
    {
        NoData.Broadcast(nullptr);
        return;
    }

    const TArray<UCardObject*>& SelectedCards = SelectionManager->GetSelectedCards();

    // Disable glow for cards not in SelectedCards
    for (const auto& CardPair : CardManager->ActorToCardMap)
    {
        A3DCard* CardActor = CardPair.Key;
        UCardObject* CardObject = CardPair.Value;

        // also check that the Hit Actor is not the CardActor
        if (CardObject && !SelectedCards.Contains(CardObject) && CardActor != HitActor)
        {
            CardObject->Execute_ToggleCardGlow(CardObject, false, FLinearColor::Green);
        }
    }

    if (HitActor && HitActor->GetClass()->ImplementsInterface(Interface))
    {
        if (const A3DCard* CardActor = Cast<A3DCard>(HitActor))
        {
            if (UCardObject* CardObject = CardActor->GetCardObject())
            {
                NewData.Broadcast(CardObject);
                return;
            }
        }
    }
    
    NoData.Broadcast(nullptr);
}
