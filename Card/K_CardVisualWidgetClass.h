// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "K_CardType.h"
#include "K_CardVisualWidgetClass.generated.h"

class UBorder;
class UCardObject;

/**
 * Provides visual representation for card details.
 * Updates UI elements based on card object data.
 */

UCLASS(Abstract, Blueprintable, BlueprintType)
class UK_VisualWidgetClass : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	// Binding text components for card details
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_CardName;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_CardDescription;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_CardType;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_CoreValue;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_DamageValue;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_MitigationValue;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_RestoreValue;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_PowerValue;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_MaxUsesPerTurn;

	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_RemainingUses;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_Portrait;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_CardFrame;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_CoreFrame;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_DamageFrame;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_MitigationFrame;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_RestoreFrame;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Image_PowerFrame;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* GlowBorder; // Assuming you have a UBorder widget for the card border
    
	// Method to update all UI elements based on current data
	UFUNCTION(BlueprintCallable, Category = "UI Update")
	void UpdateCardWidget(UCardObject* Card);

	// Method to toggle the border glow
	UFUNCTION(BlueprintCallable, Category = "UI Update")
	void ToggleCardGlow(bool bEnable, FLinearColor GlowColor);

private:
	// Helper function to convert card type enum to string
	static FString ConvertCardTypeToString(ECardType Type);
};
