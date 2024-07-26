// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "K_GameMetricsWidget.generated.h"

class UMaterialInstance;
class UMaterialInstanceDynamic;
class ALyraPlayerState;
class UImage;

UCLASS(Abstract, Blueprintable, BlueprintType)
class K_CARDSRUNTIME_API UK_GameMetricsWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void InitializeWithPlayerStateComponent(UK_CardsPlayerStateComponent* PlayerStateComponent);

	// bind attributes to text fields
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CommonTextBlock_Days;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CommonTextBlock_HandLimit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CommonTextBlock_Exposure;

	void UpdateHandLimitText(float InitialHandLimit) const;
	void UpdateExposureText(float InitialExposure) const;
	void BindAttributes();

	TWeakObjectPtr<UK_CardsPlayerStateComponent> PlayerStateComponent;

	void UpdateTextBlock(const FOnAttributeChangeData& OnAttributeChangeData, TObjectPtr<UTextBlock> TextBlock);
	void BindAttributeChange(const FGameplayAttribute& Attribute, TObjectPtr<UTextBlock> TextBlock);

	UPROPERTY()
	ALyraPlayerState* BoundPlayerState = nullptr;
};
