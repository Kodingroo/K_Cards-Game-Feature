// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "System/LyraGameInstance.h"
#include "K_CardsGameInstance.generated.h"

class UUserWidget;

/**
 * Custom game instance for managing game-wide logic and states.
 * Handles game initialization, shutdown, and game over screen display.
 */

UCLASS(Config = Game)
class K_CARDSRUNTIME_API UK_CardsGameInstance : public ULyraGameInstance
{
	GENERATED_BODY()

public:
	UK_CardsGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void ShowGameOverScreen();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Over")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void Init() override;
	virtual void Shutdown() override;
};
