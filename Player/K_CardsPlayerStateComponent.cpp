// Copyright Isaac Hayward

#include "K_CardsPlayerStateComponent.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "System/K_CardManager.h"
#include "Player/LyraPlayerState.h"

void UK_CardsPlayerStateComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UK_CardsPlayerStateComponent::OnRegister()
{
    Super::OnRegister();
    InitializeAttributes();
}

void UK_CardsPlayerStateComponent::InitializeAttributes()
{
    if (ALyraPlayerState* PlayerState = GetOwner<ALyraPlayerState>())
    {
        if (ULyraAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetLyraAbilitySystemComponent())
        {
            for (const FAttributeDefaults& StartingData : DefaultStartingAttributeData)
            {
                if (StartingData.Attributes && StartingData.DefaultStartingTable)
                {
                    UAttributeSet* Attributes = NewObject<UAttributeSet>(this, StartingData.Attributes);
                    if (Attributes)
                    {
                        AbilitySystemComponent->AddAttributeSetSubobject(Attributes);
                        Attributes->InitFromMetaDataTable(StartingData.DefaultStartingTable);
                    }
                }
            }
        }
    }
}

void UK_CardsPlayerStateComponent::SetupStartingDeckData(UDataTable* DataTable, TSubclassOf<UCardObject> CardClass, int32 HandSize)
{
    StartingDeckDataTable = DataTable;
    StartingCardClass = CardClass;
    StartingHandSize = HandSize;
}

void UK_CardsPlayerStateComponent::SetupStartingPlayerDeck()
{
    if (StartingDeckDataTable && StartingCardClass)
    {
        SetupPlayerDeck(StartingDeckDataTable, StartingCardClass, StartingHandSize);
    }
}

void UK_CardsPlayerStateComponent::AddCardToDeck(UCardObject* Card)
{
    if (!Card)
    {
        return;
    }
    
    if (!PlayerDeck.Contains(Card))
    {
        PlayerDeck.Add(Card);
    }
    
    Card->CardState = ECardState::Passive;
    Card->Execute_SetCurrentCardPile(Card, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Discard")));
    AddCardToPile(Card, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Discard")));
    // Add card to discard pile by default when added from the table
    OnCardTransferred.Broadcast(Card, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Draw")), FGameplayTag::RequestGameplayTag(FName("Card.Pile.Discard")));
    OnRep_PlayerDeck();
}

void UK_CardsPlayerStateComponent::RemoveCardFromDeck(UCardObject* Card)
{
    if (PlayerDeck.Contains(Card))
    {
        PlayerDeck.RemoveSingle(Card);
        RemoveSpecificCardFromPile(Card, Card->CurrentPileTag);
        Card->CardState = ECardState::Passive;
        Card->Execute_SetCurrentCardPile(Card, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Burn")));
        OnRep_PlayerDeck();
    }
}

void UK_CardsPlayerStateComponent::ClearDeck()
{
    for (UCardObject* Card : PlayerDeck)
    {
        if (IsValid(Card))
        {
            Card->CardState = ECardState::Passive;
        }
    }
    CardPileMap.Empty();
    PlayerDeck.Empty();
    OnRep_PlayerDeck();
}

void UK_CardsPlayerStateComponent::SetupPlayerDeck(UDataTable* CardDataTable, TSubclassOf<UCardObject> CardClass, int32 InitialHandSize)
{
    if (!CardDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetupPlayerDeck - CardDataTable is null"));
        return;
    }
    
    ClearDeck();

    TArray<FName> RowNames = CardDataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FCardData* CardData = CardDataTable->FindRow<FCardData>(RowName, "");
        if (CardData)
        {
            if (UCardObject* NewCardObject = UK_CardManager::GetInstance()->CreateAndPopulateCardObject(CardClass, *CardData, ECardState::Passive, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Draw"))))
            {
                AddCardToDeck(NewCardObject);
            }
        }
    }
    
    if (ALyraPlayerState* PlayerState = GetOwner<ALyraPlayerState>())
    {
        if (ULyraAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetLyraAbilitySystemComponent())
        {
            FGameplayTag AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Deck.Build"));
            AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));
        }
    }

    OnRep_PlayerDeck(); // Ensure the deck is updated
}

void UK_CardsPlayerStateComponent::TransferCardToPile(UCardObject* Card, FGameplayTag NewPileTag)
{
    if (Card && HasAuthority())
    {
        FGameplayTag InitialPileTag = Card->CurrentPileTag;
        RemoveSpecificCardFromPile(Card, InitialPileTag);
        AddCardToPile(Card, NewPileTag);
        Card->Execute_SetCurrentCardPile(Card, NewPileTag);
        OnCardTransferred.Broadcast(Card, InitialPileTag, NewPileTag);
    }
    else if (Card)
    {
        ServerTransferCardToPile(Card, NewPileTag);
    }
}

void UK_CardsPlayerStateComponent::ServerTransferCardToPile_Implementation(UCardObject* Card, FGameplayTag NewPileTag)
{
    TransferCardToPile(Card, NewPileTag);
}

bool UK_CardsPlayerStateComponent::ServerTransferCardToPile_Validate(UCardObject* Card, FGameplayTag NewPileTag)
{
    return true;
}

void UK_CardsPlayerStateComponent::AddCardToPile(UCardObject* Card, const FGameplayTag& PileTag)
{
    // check if the card already exists in the pile, otherwise we will end up with blank card slots in hand.
    if (CardPileMap.Contains(PileTag) && CardPileMap[PileTag].Contains(Card))
    {
        return;
    }
    
    if (Card)
    {
        CardPileMap.FindOrAdd(PileTag).Add(Card);
    }

    OnCardTransferred.Broadcast(Card, FGameplayTag::RequestGameplayTag(FName("Card.Pile.Draw")), PileTag);
}

UCardObject* UK_CardsPlayerStateComponent::PopCardFromPile(const FGameplayTag& PileTag)
{
    if (CardPileMap.Contains(PileTag) && CardPileMap[PileTag].Num() > 0)
    {
        return CardPileMap[PileTag].Pop();
    }
    return nullptr;
}

UCardObject* UK_CardsPlayerStateComponent::RemoveSpecificCardFromPile(UCardObject* Card, const FGameplayTag& PileTag)
{
    if (TArray<UCardObject*>* Pile = CardPileMap.Find(PileTag))
    {
        int32 CardIndex = Pile->Find(Card);
        if (CardIndex != INDEX_NONE)
        {
            Pile->RemoveAt(CardIndex);
            return Card;
        }
    }
    return nullptr;
}

int32 UK_CardsPlayerStateComponent::GetCardCountInPile(const FGameplayTag& PileTag) const
{
    if (const TArray<UCardObject*>* Pile = CardPileMap.Find(PileTag))
    {
        return Pile->Num();
    }
    return 0;
}

TArray<UCardObject*>& UK_CardsPlayerStateComponent::GetCardsInPile(const FGameplayTag& PileTag)
{
    return CardPileMap.FindOrAdd(PileTag);
}

TMap<FGameplayTag, TArray<UCardObject*>> UK_CardsPlayerStateComponent::GetAllCardPiles() const
{
    return CardPileMap;
}

void UK_CardsPlayerStateComponent::ShuffleDrawPile()
{
    FGameplayTag DrawPileTag = FGameplayTag::RequestGameplayTag(FName("Card.Pile.Draw"));
    FGameplayTag DiscardPileTag = FGameplayTag::RequestGameplayTag(FName("Card.Pile.Discard"));

    // Get the draw and discard piles
    TArray<UCardObject*>& DrawPile = CardPileMap.FindOrAdd(DrawPileTag);
    TArray<UCardObject*>& DiscardPile = CardPileMap.FindOrAdd(DiscardPileTag);

    // If draw pile is empty, move all cards from discard pile to draw pile
    if (DrawPile.Num() == 0 && DiscardPile.Num() > 0)
    {
        DrawPile.Append(DiscardPile);
        DiscardPile.Empty();
    }

    // Shuffle the draw pile
    if (DrawPile.Num() > 0)
    {
        for (int32 i = DrawPile.Num() - 1; i > 0; --i)
        {
            int32 j = FMath::RandRange(0, i);
            DrawPile.Swap(i, j);
        }
    }  
}

void UK_CardsPlayerStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UK_CardsPlayerStateComponent, PlayerDeck);
    DOREPLIFETIME(UK_CardsPlayerStateComponent, PlayerID);
    DOREPLIFETIME(UK_CardsPlayerStateComponent, Initiative);
}

void UK_CardsPlayerStateComponent::OnRep_PlayerDeck()
{
    // Handle logic post deck replication, e.g., update UI.
}

void UK_CardsPlayerStateComponent::OnRep_PlayerAttributes()
{
    // Handle logic post attributes replication, e.g., update UI.
}
