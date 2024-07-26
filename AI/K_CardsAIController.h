#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Card/K_CardObject.h"
#include "K_CardsAIController.generated.h"

UCLASS()
class K_CARDSRUNTIME_API AK_CardsAIController : public AAIController
{
	GENERATED_BODY()

public:
	AK_CardsAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<UCardObject*> AIDeck;
	
	void GetAIDeck(TArray<UCardObject*>& OutAIDeck) const;

	/** Remove a card object from the AI Deck */
	UFUNCTION(BlueprintCallable, Category = "Card Management")
	void RemoveCardFromAIDeck(UCardObject* Card);

protected:
	virtual void BeginPlay() override;
};

