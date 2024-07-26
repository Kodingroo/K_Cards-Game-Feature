// Copyright Isaac Hayward

#include "K_3DCard.h"

#include "K_CardObject.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameplayMessageSubsystem.h"

A3DCard::A3DCard()
    : CardObject(nullptr), MovementCurve(nullptr), ElapsedTime(0.0f), MoveDuration(0), InteractionVolume(nullptr)
{
    PrimaryActorTick.bCanEverTick = true;
}

void A3DCard::BeginPlay()
{
    Super::BeginPlay();
}

void A3DCard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CardObject && CardObject->CardState == ECardState::FollowMouse)
    {
        FVector MousePosition, MouseDirection;
        APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
        if (PlayerController)
        {
            PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection);

            FVector NewLocation = MousePosition + DragOffset;
            if (IsWithinBounds(NewLocation))
            {
                SetActorLocation(NewLocation);
            }
        }
    }
    else if (CardObject && CardObject->CardState == ECardState::InTransit && MovementCurve)
    {
        ElapsedTime += DeltaTime;
        float LerpValue = MovementCurve->GetFloatValue(ElapsedTime / MoveDuration);

        FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, LerpValue);
        SetActorLocation(NewLocation);

        if (LerpValue >= 1.0f)
        {
            CardObject->CardState = ECardState::OnTable;
            OnArrivedAtDestination();
        }
    }
}

void A3DCard::SetCardObject(UCardObject* InCardObject)
{
    CardObject = InCardObject;
}

UCardObject* A3DCard::GetCardObject() const
{
    return CardObject;
}

void A3DCard::EnableDrag()
{
    if (CardObject)
    {
        CardObject->CardState = ECardState::FollowMouse;

        FVector MousePosition, MouseDirection;
        APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
        if (PlayerController)
        {
            PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection);
            DragOffset = GetActorLocation() - MousePosition;
        }
    }
}

void A3DCard::DisableDrag()
{
    if (CardObject)
    {
        CardObject->CardState = ECardState::OnTable;
    }
}

void A3DCard::MoveCardToLocation(const FVector& Destination, UCurveFloat* LerpCurve, float Duration)
{
    if (!CardObject || !LerpCurve)
    {
        return;
    }

    StartLocation = GetActorLocation();
    TargetLocation = Destination;
    MovementCurve = LerpCurve;
    ElapsedTime = 0.0f;
    MoveDuration = Duration;
    CardObject->CardState = ECardState::InTransit;
}

void A3DCard::OnArrivedAtDestination()
{
    if (CardObject)
    {
        BroadcastArrival();
    }
}

void A3DCard::BroadcastArrival()
{
    FMessage_ArrivedAtDestination Message;
    Message.CardActor = this;

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
    MessageSubsystem.BroadcastMessage(FGameplayTag::RequestGameplayTag(FName("Card.Message.ArrivedAtDestination")), Message);
}

void A3DCard::InitializeInteractionVolume(AVolume* InVolume)
{
    InteractionVolume = InVolume;
}

bool A3DCard::IsWithinBounds(const FVector& Location) const
{
    if (!InteractionVolume)
    {
        return true; // Allow movement if there is no defined interaction volume
    }

    FBox VolumeBox = InteractionVolume->GetComponentsBoundingBox();
    return VolumeBox.IsInside(Location);
}
