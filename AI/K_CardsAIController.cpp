#include "K_CardsAIController.h"

AK_CardsAIController::AK_CardsAIController()
{
	bWantsPlayerState = true;
}

void AK_CardsAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AK_CardsAIController::GetAIDeck(TArray<UCardObject*>& OutAIDeck) const
{
	OutAIDeck = AIDeck;
}

void AK_CardsAIController::RemoveCardFromAIDeck(UCardObject* Card)
{
	if (Card)
	{
		AIDeck.Remove(Card);
	}
}
