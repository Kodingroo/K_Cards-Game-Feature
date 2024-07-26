// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "K_CardObjectInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCardObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface should be implemented by classes that represent card objects.
 */

class ICardObjectInterface
{
	GENERATED_BODY()

public:
	// Getters
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Card Management")
	FGameplayTag GetCurrentCardPile() const;
	virtual FGameplayTag GetCurrentCardPile_Implementation() const = 0;

	// Setters
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Card Management")
	void SetCurrentCardPile(const FGameplayTag& PileName);
	virtual void SetCurrentCardPile_Implementation(const FGameplayTag& PileName) = 0;

	// Toggle Card Glow
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Card Management")
	void ToggleCardGlow(bool bEnable, FLinearColor GlowColor);
	virtual void ToggleCardGlow_Implementation(bool bEnable, FLinearColor GlowColor) = 0;
};

// Inline function implementations (if needed)
inline FGameplayTag ICardObjectInterface::GetCurrentCardPile_Implementation() const
{
	// This is a pure virtual function. It must be implemented in a derived class.
	check(0);
	return FGameplayTag();
}

inline void ICardObjectInterface::SetCurrentCardPile_Implementation(const FGameplayTag& PileName)
{
	// This is a pure virtual function. It must be implemented in a derived class.
	check(0);
}

inline void ICardObjectInterface::ToggleCardGlow_Implementation(bool bEnable, FLinearColor GlowColor)
{
	// This is a pure virtual function. It must be implemented in a derived class.
	check(0);
}
