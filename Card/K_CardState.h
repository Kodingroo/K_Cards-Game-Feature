// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "K_CardState.generated.h"

UENUM(BlueprintType)
enum class ECardState : uint8
{
	Activating UMETA(DisplayName = "Activating"),
	FollowBezier UMETA(DisplayName = "Follow Bezier"),
	FollowMouse UMETA(DisplayName = "Follow Mouse"),
	InHand UMETA(DisplayName = "In Hand"),
	InTransit UMETA(DisplayName = "In Transit"),
	MouseOver UMETA(DisplayName = "Mouse Over"),
	OnTable UMETA(DisplayName = "On Table"),
	Passive UMETA(DisplayName = "Passive"),
	Selected UMETA(DisplayName = "Selected"),
	Targeting UMETA(DisplayName = "Targeting")
};
