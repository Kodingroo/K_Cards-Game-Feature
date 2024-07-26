// Copyright Isaac Hayward

#include "UI/K_GameMetricsWidget.h"
#include "Player/K_CardsPlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player/LyraPlayerState.h"

void UK_GameMetricsWidget::InitializeWithPlayerStateComponent(UK_CardsPlayerStateComponent* InPlayerStateComponent)
{
	PlayerStateComponent = InPlayerStateComponent;

	if (PlayerStateComponent.IsValid())
	{
		BindAttributes();
	}
}

void UK_GameMetricsWidget::BindAttributes()
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
			BindAttributeChange(UK_CardsPlayerAttributeSet::GetHandLimitAttribute(), CommonTextBlock_HandLimit);
			BindAttributeChange(UK_CardsPlayerAttributeSet::GetExposureDamageAttribute(), CommonTextBlock_Exposure);

			// Initial update
			float InitialHandLimit = AbilitySystemComponent->GetNumericAttribute(UK_CardsPlayerAttributeSet::GetHandLimitAttribute());
			float InitialExposure = AbilitySystemComponent->GetNumericAttribute(UK_CardsPlayerAttributeSet::GetExposureDamageAttribute());
			
			FOnAttributeChangeData InitialHandLimitData;
			InitialHandLimitData.NewValue = InitialHandLimit;

			FOnAttributeChangeData InitialExposureData;
			InitialExposureData.NewValue = InitialExposure;
			
			UpdateTextBlock(InitialHandLimitData, CommonTextBlock_HandLimit);
			UpdateTextBlock(InitialExposureData, CommonTextBlock_Exposure);
		}
	}
}

void UK_GameMetricsWidget::UpdateHandLimitText(float InitialHandLimit) const
{
	if (CommonTextBlock_HandLimit)
	{
		CommonTextBlock_HandLimit->SetText(FText::FromString(FString::Printf(TEXT("Current hand limit: %d"), FMath::RoundToInt(InitialHandLimit))));
	}
}

void UK_GameMetricsWidget::UpdateExposureText(float InitialExposure) const
{
	const float Hunger = PlayerStateComponent->GetOwner<ALyraPlayerState>()->GetLyraAbilitySystemComponent()->GetNumericAttribute(UK_CardsPlayerAttributeSet::GetHungerAttribute());

	int32 TruncatedExposure = FMath::FloorToInt(InitialExposure);
	FString ExposureString = FString::Printf(TEXT("%s%d health from exposure and %s%d health from hunger status each sleep"), 
											 (TruncatedExposure > 0 ? TEXT("-") : TEXT("")), TruncatedExposure,
											 (Hunger >= 0 ? TEXT("+") : TEXT("-")), 5);
	CommonTextBlock_Exposure->SetText(FText::FromString(ExposureString));
}


void UK_GameMetricsWidget::UpdateTextBlock(const FOnAttributeChangeData& OnAttributeChangeData,
                                           TObjectPtr<UTextBlock> TextBlock)
{
	if (TextBlock == CommonTextBlock_HandLimit)
	{
		UpdateHandLimitText(OnAttributeChangeData.NewValue);
	}
	if (TextBlock == CommonTextBlock_Exposure)
	{
		UpdateExposureText(OnAttributeChangeData.NewValue);
	}
}

void UK_GameMetricsWidget::BindAttributeChange(const FGameplayAttribute& Attribute, TObjectPtr<UTextBlock> TextBlock)
{
	if (BoundPlayerState)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = BoundPlayerState->GetLyraAbilitySystemComponent())
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UK_GameMetricsWidget::UpdateTextBlock, TextBlock);
		}
	}
}