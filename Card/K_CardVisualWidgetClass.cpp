// Copyright Isaac Hayward

#include "K_CardVisualWidgetClass.h"
#include "K_CardObject.h"
#include "Components/Border.h"

void UK_VisualWidgetClass::UpdateCardWidget(UCardObject* Card)
{
	if (!Card)
		return;

	if (CommonTextBlock_CardName)
		CommonTextBlock_CardName->SetText(Card->Name);
	
	if (CommonTextBlock_CardDescription)
		CommonTextBlock_CardDescription->SetText(Card->Description);

	if (CommonTextBlock_CardType)
		CommonTextBlock_CardType->SetText(FText::FromString(ConvertCardTypeToString(Card->Type)));

	if (CommonTextBlock_CoreValue)
		CommonTextBlock_CoreValue->SetText(FText::AsNumber(Card->CoreValue));

	if (CommonTextBlock_DamageValue)
		CommonTextBlock_DamageValue->SetText(FText::AsNumber(Card->DamageValue));

	if (CommonTextBlock_MitigationValue)
		CommonTextBlock_MitigationValue->SetText(FText::AsNumber(Card->MitigationValue));

	if (CommonTextBlock_RestoreValue)
		CommonTextBlock_RestoreValue->SetText(FText::AsNumber(Card->RestoreValue));

	if (CommonTextBlock_PowerValue)
		CommonTextBlock_PowerValue->SetText(FText::AsNumber(Card->PowerValue));
	
	if (CommonTextBlock_MaxUsesPerTurn)
		CommonTextBlock_MaxUsesPerTurn->SetText(FText::AsNumber(Card->MaxUsesPerTurn));

	if (CommonTextBlock_RemainingUses)
		CommonTextBlock_RemainingUses->SetText(FText::AsNumber(Card->RemainingUses));

	if (Image_Portrait)
		Image_Portrait->SetBrushFromTexture(Card->Portrait);

	if (Image_CardFrame)
		Image_CardFrame->SetBrushFromTexture(Card->CardFrame);

	if (Image_CoreFrame)
		Image_CoreFrame->SetBrushFromTexture(Card->CoreFrame);

	if (Image_DamageFrame)
		Image_DamageFrame->SetBrushFromTexture(Card->DamageFrame);

	if (Image_MitigationFrame)
		Image_MitigationFrame->SetBrushFromTexture(Card->MitigationFrame);

	if (Image_RestoreFrame)
		Image_RestoreFrame->SetBrushFromTexture(Card->RestoreFrame);

	if (Image_PowerFrame)
		Image_PowerFrame->SetBrushFromTexture(Card->PowerFrame);
}

FString UK_VisualWidgetClass::ConvertCardTypeToString(ECardType Type)
{
	const UEnum* EnumPtr = StaticEnum<ECardType>();
	if (!EnumPtr)
		return FString("Invalid Type");

	return EnumPtr->GetNameStringByValue(static_cast<int64>(Type));
}

void UK_VisualWidgetClass::ToggleCardGlow(bool bEnable, FLinearColor GlowColor)
{
	if (GlowBorder)
	{
		if (bEnable)
		{
			GlowBorder->SetBrushColor(GlowColor);
		}
		else
		{
			GlowBorder->SetBrushColor(FLinearColor::Transparent);
		}
	}
}