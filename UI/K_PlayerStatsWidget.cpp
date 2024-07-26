// Copyright Isaac Hayward

#include "K_PlayerStatsWidget.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player/LyraPlayerState.h"
#include "Components/Image.h"

void UK_PlayerStatsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Create dynamic material instances from the base materials set in the designer
    if (HealthMaterial)
    {
        HealthMaterialDynamic = UMaterialInstanceDynamic::Create(HealthMaterial, this);
        if (Image_Health)
        {
            Image_Health->SetBrushFromMaterial(HealthMaterialDynamic);
        }
    }

    if (SanityMaterial)
    {
        SanityMaterialDynamic = UMaterialInstanceDynamic::Create(SanityMaterial, this);
        if (Image_Sanity)
        {
            Image_Sanity->SetBrushFromMaterial(SanityMaterialDynamic);
        }
    }

    if (HungerMaterial)
    {
        HungerMaterialDynamic = UMaterialInstanceDynamic::Create(HungerMaterial, this);
        if (Image_Hunger)
        {
            Image_Hunger->SetBrushFromMaterial(HungerMaterialDynamic);
        }
    }
}

void UK_PlayerStatsWidget::InitializeWithPlayerStateComponent(UK_CardsPlayerStateComponent* InPlayerStateComponent)
{
    PlayerStateComponent = InPlayerStateComponent;

    if (PlayerStateComponent.IsValid())
    {
        BindAttributes();
    }
}

void UK_PlayerStatsWidget::BindAttributes()
{
    if (!PlayerStateComponent.IsValid())
    {
        return;
    }

    if (ALyraPlayerState* LyraPlayerState = Cast<ALyraPlayerState>(PlayerStateComponent->GetOwner()))
    {
        BoundPlayerState = LyraPlayerState;
        UAbilitySystemComponent* AbilitySystemComponent = LyraPlayerState->GetLyraAbilitySystemComponent();

        if (AbilitySystemComponent)
        {
            // Bind to attribute changes
            BindAttributeChange(UK_CardsPlayerAttributeSet::GetHealthAttribute(), HealthMaterialDynamic);
            BindAttributeChange(UK_CardsPlayerAttributeSet::GetSanityAttribute(), SanityMaterialDynamic);
            BindAttributeChange(UK_CardsPlayerAttributeSet::GetHungerAttribute(), HungerMaterialDynamic);

            // Initial update
            float InitialHealth = AbilitySystemComponent->GetNumericAttribute(UK_CardsPlayerAttributeSet::GetHealthAttribute());
            float InitialSanity = AbilitySystemComponent->GetNumericAttribute(UK_CardsPlayerAttributeSet::GetSanityAttribute());
            float InitialHunger = AbilitySystemComponent->GetNumericAttribute(UK_CardsPlayerAttributeSet::GetHungerAttribute());
            
            FOnAttributeChangeData InitialHealthData;
            InitialHealthData.NewValue = InitialHealth;

            FOnAttributeChangeData InitialSanityData;
            InitialSanityData.NewValue = InitialSanity;

            FOnAttributeChangeData InitialHungerData;
            InitialHungerData.NewValue = InitialHunger;

            UpdateMaterialFillAmount(InitialHealthData, HealthMaterialDynamic);
            UpdateMaterialFillAmount(InitialSanityData, SanityMaterialDynamic);
            UpdateMaterialFillAmount(InitialHungerData, HungerMaterialDynamic);
        }
    }
}

void UK_PlayerStatsWidget::UpdateMaterialFillAmount(const FOnAttributeChangeData& Data, UMaterialInstanceDynamic* Material)
{
    if (Material)
    {
        Material->SetScalarParameterValue(FName("FillAmount"), Data.NewValue);
    }
}

void UK_PlayerStatsWidget::BindAttributeChange(const FGameplayAttribute& Attribute, UMaterialInstanceDynamic* Material)
{
    if (BoundPlayerState)
    {
        if (UAbilitySystemComponent* AbilitySystemComponent = BoundPlayerState->GetLyraAbilitySystemComponent())
        {
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UK_PlayerStatsWidget::UpdateMaterialFillAmount, Material);
        }
    }
}
