// Copyright Isaac Hayward

#pragma once

#include "K_CardType.generated.h"

UENUM(BlueprintType)
enum class ECardType : uint8
{
	Accessory UMETA(DisplayName = "Accessory"),
	Action UMETA(DisplayName = "Action"),
	Armor UMETA(DisplayName = "Armor"),
	Artifact UMETA(DisplayName = "Artifact"),
	Character UMETA(DisplayName = "Character"),
	Consumable UMETA(DisplayName = "Consumable"),
	Creature UMETA(DisplayName = "Creature"),
	Enchantment UMETA(DisplayName = "Enchantment"),
	Location UMETA(DisplayName = "Location"),
	MainStay UMETA(DisplayName = "MainStay"),
	Resource UMETA(DisplayName = "Resource"),
	Spell UMETA(DisplayName = "Spell"),
	Structure UMETA(DisplayName = "Structure"),
	Tool UMETA(DisplayName = "Tool"),
	Trap UMETA(DisplayName = "Trap"),
	Weapon UMETA(DisplayName = "Weapon")
};