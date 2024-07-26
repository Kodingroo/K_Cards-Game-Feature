// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "K_CardSearchTagContainer.generated.h"

UENUM(BlueprintType)
enum class ECardTagContainer : uint8
{
	CardTags UMETA(DisplayName = "Card Tags"),
	InteractionConditions UMETA(DisplayName = "Interaction Conditions"),
	PlaySettings UMETA(DisplayName = "Play Settings"),
	Purpose UMETA(DisplayName = "Purpose")
};
