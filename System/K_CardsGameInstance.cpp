// Copyright Isaac Hayward

#include "K_CardsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "K_CardManager.h"

UK_CardsGameInstance::UK_CardsGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UK_CardsGameInstance::Init()
{
	Super::Init();
	UK_CardManager::Initialize();
}

void UK_CardsGameInstance::Shutdown()
{
	UK_CardManager::Shutdown();
	Super::Shutdown();
}

void UK_CardsGameInstance::ShowGameOverScreen()
{
	if (GameOverWidgetClass)
	{
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport();
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}
