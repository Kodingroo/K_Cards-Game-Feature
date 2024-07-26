// Copyright Isaac Hayward

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Player/K_CardsPlayerStateComponent.h"
#include "K_PlayerStatsWidget.generated.h"

class UMaterialInstance;
class UMaterialInstanceDynamic;
class ALyraPlayerState;
class UImage;

UCLASS(Abstract, Blueprintable, BlueprintType)
class K_CARDSRUNTIME_API UK_PlayerStatsWidget : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Player Stats")
    void InitializeWithPlayerStateComponent(UK_CardsPlayerStateComponent* PlayerStateComponent);

protected:
    virtual void NativeConstruct() override;

    // Base material instances to be set in the designer
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    TObjectPtr<UMaterialInstance> HealthMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    TObjectPtr<UMaterialInstance> SanityMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    TObjectPtr<UMaterialInstance> HungerMaterial;

    // Images to bind in the designer
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Image_Health;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Image_Sanity;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Image_Hunger;

private:
    void BindAttributes();
    void UpdateMaterialFillAmount(const FOnAttributeChangeData& ChangeData, UMaterialInstanceDynamic* Material);

    TWeakObjectPtr<UK_CardsPlayerStateComponent> PlayerStateComponent;

    void BindAttributeChange(const FGameplayAttribute& Attribute, UMaterialInstanceDynamic* Material);

    UPROPERTY()
    ALyraPlayerState* BoundPlayerState = nullptr;

    // Dynamic material instances created at runtime
    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> HealthMaterialDynamic;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> SanityMaterialDynamic;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> HungerMaterialDynamic;
};
