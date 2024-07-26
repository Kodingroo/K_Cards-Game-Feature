// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "K_CardState.h"
#include "K_3DCard.generated.h"

class A3DCard;

USTRUCT(BlueprintType)
struct FMessage_ArrivedAtDestination
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Message")
	A3DCard* CardActor;
};

class UCardObject;

/**
 * Represents a 3D card actor with functionality for movement and interaction.
 */

UCLASS()
class K_CARDSRUNTIME_API A3DCard : public AActor
{
	GENERATED_BODY()

public:
	A3DCard();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	// Function to set the card object
	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetCardObject(UCardObject* InCardObject);

	// Function to get the card object
	UFUNCTION(BlueprintCallable, Category = "Card")
	UCardObject* GetCardObject() const;

	// Enable card dragging
	UFUNCTION(BlueprintCallable, Category = "Card")
	void EnableDrag();

	// Disable card dragging
	UFUNCTION(BlueprintCallable, Category = "Card")
	void DisableDrag();

	// Function to move the card to a specified location
	UFUNCTION(BlueprintCallable, Category = "Card Movement")
	void MoveCardToLocation(const FVector& Destination, UCurveFloat* LerpCurve, float Duration);

	// Initialize the interaction volume
	void InitializeInteractionVolume(AVolume* InVolume);

private:
	// The associated card object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card", meta = (AllowPrivateAccess = "true"))
	UCardObject* CardObject;

	// Movement variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Movement", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MovementCurve;

	FVector StartLocation;
	FVector TargetLocation;
	float ElapsedTime;
	float MoveDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Movement", meta = (AllowPrivateAccess = "true"))
	AVolume* InteractionVolume;
	FVector DragOffset;

	// Function called when the card arrives at its destination
	void OnArrivedAtDestination();

	// Broadcasts the arrival message
	void BroadcastArrival();

	// Check if the card is within the bounds
	bool IsWithinBounds(const FVector& Location) const;
};
