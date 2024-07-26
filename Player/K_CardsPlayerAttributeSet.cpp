// Copyright Isaac Hayward

#include "K_CardsPlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Kismet/GameplayStatics.h"
#include "System/K_CardsGameInstance.h"

UK_CardsPlayerAttributeSet::UK_CardsPlayerAttributeSet()
    : Health(100.f), Sanity(100.f), Hunger(10.f), HandLimit(3), ExposureDamage(-10.f)
{
}

void UK_CardsPlayerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UK_CardsPlayerAttributeSet, Health, OldHealth);
}

void UK_CardsPlayerAttributeSet::OnRep_Sanity(const FGameplayAttributeData& OldSanity)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UK_CardsPlayerAttributeSet, Sanity, OldSanity);
}

void UK_CardsPlayerAttributeSet::OnRep_Hunger(const FGameplayAttributeData& OldHunger)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UK_CardsPlayerAttributeSet, Hunger, OldHunger);
}

void UK_CardsPlayerAttributeSet::OnRep_HandLimit(const FGameplayAttributeData& OldHandLimit)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UK_CardsPlayerAttributeSet, HandLimit, OldHandLimit);
}

void UK_CardsPlayerAttributeSet::OnRep_ExposureDamage(const FGameplayAttributeData& OldExposureDamage)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UK_CardsPlayerAttributeSet, ExposureDamage, OldExposureDamage);
}

void UK_CardsPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        float NewHealth = Health.GetCurrentValue();
        if (NewHealth <= 0.0f)
        {
            // Die!
            Health.SetBaseValue(0.0f);
            Health.SetCurrentValue(0.0f);
            HandleHealthZero();
        }
    }
}

void UK_CardsPlayerAttributeSet::HandleHealthZero()
{
    if (UWorld* World = GetWorld())
    {
        UK_CardsGameInstance* GameInstance = Cast<UK_CardsGameInstance>(UGameplayStatics::GetGameInstance(World));
        if (GameInstance)
        {
            GameInstance->ShowGameOverScreen();
        }
    }
}

void UK_CardsPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UK_CardsPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UK_CardsPlayerAttributeSet, Sanity, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UK_CardsPlayerAttributeSet, Hunger, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UK_CardsPlayerAttributeSet, HandLimit, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UK_CardsPlayerAttributeSet, ExposureDamage, COND_None, REPNOTIFY_Always);
}
