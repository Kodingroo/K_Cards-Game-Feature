// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "K_CardsPlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Defines player attributes such as health, sanity, and hunger.
 * Manages replication and handles gameplay effect executions.
 */

UCLASS()
class K_CARDSRUNTIME_API UK_CardsPlayerAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UK_CardsPlayerAttributeSet();

    // Player stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UK_CardsPlayerAttributeSet, Health)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Sanity, Category = "Attributes")
    FGameplayAttributeData Sanity;
    ATTRIBUTE_ACCESSORS(UK_CardsPlayerAttributeSet, Sanity)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Hunger, Category = "Attributes")
    FGameplayAttributeData Hunger;
    ATTRIBUTE_ACCESSORS(UK_CardsPlayerAttributeSet, Hunger)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_HandLimit, Category = "Attributes")
    FGameplayAttributeData HandLimit;
    ATTRIBUTE_ACCESSORS(UK_CardsPlayerAttributeSet, HandLimit)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ExposureDamage, Category = "Attributes")
    FGameplayAttributeData ExposureDamage;
    ATTRIBUTE_ACCESSORS(UK_CardsPlayerAttributeSet, ExposureDamage)

protected:
    // OnRep functions for attributes
    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    virtual void OnRep_Sanity(const FGameplayAttributeData& OldSanity);

    UFUNCTION()
    virtual void OnRep_Hunger(const FGameplayAttributeData& OldHunger);

    UFUNCTION()
    virtual void OnRep_HandLimit(const FGameplayAttributeData& OldHandLimit);

    UFUNCTION()
    virtual void OnRep_ExposureDamage(const FGameplayAttributeData& OldExposureDamage);

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
    // Handles player death
    void HandleHealthZero();
};
